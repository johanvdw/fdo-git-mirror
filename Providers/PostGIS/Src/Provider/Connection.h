//
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_CONNECTION_H_INCLUDED
#define FDOPOSTGIS_CONNECTION_H_INCLUDED

#include "SchemaDescription.h"
#include "SpatialContextCollection.h"
#include "PgUtility.h"
#include <PostGIS/FdoPostGisProvider.h>
#include <PostGIS/Override/PhysicalSchemaMapping.h>
// FDO
#include <Fdo.h>
#include <FdoCommonConnPropDictionary.h>
// std
#include <string>
// libpq
#include <libpq-fe.h>

namespace fdo { namespace postgis {

// Forward declarations of internal types
class PgCursor;
class SpatialContextCollection;

/// Implementation of connection interface for PostGIS provider.
/// It represents a unique session with FDO datastore.
///
/// For every instance of the Connection class, there two states of actual
/// connection possible: closed or not closed.
/// Every function declared in public interface checks if connection is 
/// at minimum in state required for particular operation,
/// otherwise an exception is thrown.
/// 
/// \todo Make it noncopyable
///
class Connection : public FdoIConnection
{
    // NOTE - Eric Barby: Give access to schema description cache.
    // XXX - mloskot: Is it necessary? Why not to provide read-only accessor to cache?
    friend class InsertCommand;
    friend class DeleteCommand;
    friend class ApplySchemaCommand;
    friend class UpdateCommand;
    friend class SelectCommand;

public:

    /// Type of FDO smart pointer for Connection class.
    typedef FdoPtr<Connection> Ptr;

    /// Default constructor.
    Connection();

    //
    // FdoIConnection interface
    //

    /// Get pointer to FdoIConnectionCapabilities interface describing
    /// capabilities of the connection.
    FdoIConnectionCapabilities* GetConnectionCapabilities();

    /// Get pointer to FdoISchemaCapabilities interface describing
    /// support for the feature schema provided by PostGIS Provider.
    FdoISchemaCapabilities* GetSchemaCapabilities();
 	
    /// Get pointer to FdoICommandCapabilities interface describing supported commands.
    FdoICommandCapabilities* GetCommandCapabilities();

    /// Get pointer to FdoIFilterCapabilities interface describing 
    /// support for filters.
    FdoIFilterCapabilities* GetFilterCapabilities();

    /// Get pointer to FdoIExpressionCapabilities interface describing
    /// support for expressions.
    FdoIExpressionCapabilities* GetExpressionCapabilities();

    /// Get pointer to FdoIRasterCapabilities interface describing
    /// support for raster images.
    FdoIRasterCapabilities* GetRasterCapabilities();

    /// Get pointer to FdoITopologyCapabilities interface describing support for topology.
    FdoITopologyCapabilities* GetTopologyCapabilities();

    /// Get pointer to FdoIGeometryCapabilities interface describing support for geometry.
    FdoIGeometryCapabilities* GetGeometryCapabilities();

    /// Get connection string used to establish connection with a datastore.
    FdoString* GetConnectionString();

    /// Set connection string used to establish connection with a datastore.
    void SetConnectionString(FdoString* value);

    /// Get pointer to FdoIConnectionInfo interface that can be used to interrogate
    /// and set connection properties.
    FdoIConnectionInfo* GetConnectionInfo();

    /// Get state of current connection.
    FdoConnectionState GetConnectionState();

    /// Get number of milliseconds to wait while trying to establish a connection
    /// before terminating the attempt and generating an error.
    FdoInt32 GetConnectionTimeout();

    /// Set number of milliseconds to wait while trying to establish a connection
    /// before terminating the attempt and generating an error.
    void SetConnectionTimeout(FdoInt32 value);

    /// Establish connection with a datastore using settings
    /// specified by ConnectionString attribute.
    FdoConnectionState Open();

    /// Close current connection.
    void Close();

    /// Begin transaction and return pointer to FdoITransaction interface realizing
    /// transaction operations support.
    FdoITransaction* BeginTransaction();

    /// Create command object of specified type and return pointer to interface realizing
    /// command capabilities associated with the connection.
    ///
    /// \param
    /// type [in] - enumerator indicating type of requested command.
    /// \exception
    /// FdoConnectionException - if connection is closed or of invalid state.
    /// FdoException - if unsupported command is requested.
    ///
    FdoICommand* CreateCommand(FdoInt32 type);

    /// Factory function that creates an empty Schema Override mapping specific
    /// for PostGIS Provider.
    FdoPhysicalSchemaMapping* CreateSchemaMapping();

    /// Set XML configuration stream used to configure connected datastore.
    void SetConfiguration(FdoIoStream* configStream);

    /// Force to write any cached data to currently connected datastore
    void Flush();

    //
    // Connection custom interface used internally by the provider
    //
    
    /// \todo To be documented.
    FdoFeatureSchemaCollection* GetLogicalSchema();
    
    /// clears all cached schema information, next access will recache.
    void ResetSchema();

    /// \todo To be documented.
    ov::PhysicalSchemaMapping* GetPhysicalSchemaMapping();
    
    /// \todo To be documented.
    SpatialContextCollection* GetSpatialContexts();
    
    /// \todo To be documented.
    void PgExecuteCommand(char const* sql);

    /// \todo To be documented.
    void PgExecuteCommand(char const* sql, FdoSize& affected);
    
    /// \todo To be documented.
    void PgExecuteCommand(char const* sql, details::pgexec_params_t const& params, FdoSize& affected);

    /// \todo To be documented.
    PGresult* PgExecuteQuery(char const* sql);

    /// Create control object for PostgreSQL database cursor with given name.
    /// The PgCursor instance is used to define cursor using DECLARE command
    /// and can be used to retrieve a small number of rows at a time
    /// out of a larger query executed against PostGIS FDO datastore.
    /// \note All PostgreSQL cursors are read only.
    ///
    /// \param name [in] - name used to declare cursor.
    /// \return Pointer to cursor control object.
    /// \exception FdoException - state of connection instance is closed or invalid. 
    ///
    PgCursor* PgCreateCursor(char const* name);

    /// \todo To be documented.
    PGresult* PgDescribeCursor(char const* name);
    
    /// Create a transaction scope.
    /// If there is already a transaction active, calling this function
    /// does not begin a new one, but just increments the scope level counter.
    /// The idea behind soft transactions has been taken from OGR.
    ///
    /// \note PostgreSQL does not support nested transactions.
    ///
    void PgBeginSoftTransaction();
    
    /// Commit current transaction if the scope level counter hits the outer scope.
    void PgCommitSoftTransaction();
    
    /// Rollback current transaction if there is any transaction active.
    /// Rollback has effect even for nested level of transactions.
    void PgRollbackSoftTransaction();
    
    /// Force unwinding of any active transaction with commit execution.
    void PgFlushSoftTransaction();

    /// retreive the next value giving a sequence name.
    std::string GetPgNextVal(std::string sequence);
    /// Execute a query and return a single value.
    std::string PgQueryOneValue(std::string sql);
    ///Return the csName and csWkt associated to the postgis srid number
    bool GetCoordinateSystemWkt(std::string sridText,std::string& csName,std::string& csWkt);
    ///try to find a srid number giving a coordsystem name
    bool GetSrid(std::string csName,std::string& sridText);



protected:

    /// Destructor.
    virtual ~Connection();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:

    // Connection is a noncopyable type
    Connection(Connection const&);
    Connection& operator=(Connection const&);

    //
    // Data members
    //

    // Connection information object.
    // It's of lazy-creation object, delayed until first request,
    // on GetConnectionInfo() call.
    FdoPtr<FdoIConnectionInfo> mConnInfo;

    // Connection string.
    FdoStringP mConnString;

    // Connection state flag (closed, pending, open).
    FdoConnectionState mConnState;

    // Pointer to PostgreSQL connection object of current session.
    PGconn* mPgConn;

    // Pointer to PostgreSQL query result.
    PGresult* mPgResult;
    
    // Counter of soft transaction scopes.
    FdoInt32 mSoftTransactionLevel;
    
    // Keeps various schema details aggregated in common place.
    SchemaDescription::Ptr mSchemaDesc;

    //
    // Private operations
    //
    
    SchemaDescription* DescribeSchema();

    // Check internal state of connection to PostgreSQL server.
    void ValidateConnectionState();

    // Validate state of connection is not closed.
    void ValidateConnectionStateAndNotClosed();

    // Validate connection string assigned to current connection instance.
    // The function throws exception on errors detected.
    void ValidateConnectionString();

    // Validate connection properties in ConnectionInfo dictionary.
    // The function throws exception on errors detected.
    void ValidateRequiredProperties();


    // Get PostgreSQL connection parameters from FDO connection properties.
    details::pgconn_params_t GetPgConnectionParams(FdoPtr<FdoCommonConnPropDictionary> dict);

    void SetPgCurrentSchema(FdoStringP schema);

    FdoStringP GetPgCurrentSchema();
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_CONNECTION_H_INCLUDED
