/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 *
 */
#include "stdafx.h"
#include "GdbiStatement.h"
#include "GdbiCommands.h"
#include "FdoCommonOSUtil.h"


GdbiStatement::GdbiStatement(GdbiCommands* command, int qid):
m_pGdbiCommands( command )
{
	m_QueryId = new GdbiQueryIdentifier(command, qid);
}

GdbiStatement::~GdbiStatement(void)
{
	FDO_SAFE_RELEASE(m_QueryId);
}

 int GdbiStatement::GetQueryId()
 {
     return m_QueryId->GetQueryId();
 }

int GdbiStatement::Bind( int parmIndex, int size, const char* szValue, GDBI_NI_TYPE *nullInd, int typeBind)
{
	if (size == 1)
		return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_CHAR,  size, (char*)szValue, nullInd, typeBind );
	else
		return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_STRING,  size, (char*)szValue, nullInd, typeBind );
}

int GdbiStatement::Bind( int parmIndex, int size, const wchar_t* szValue, GDBI_NI_TYPE *nullInd, int typeBind)
{
	return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_WSTRING,  size, (char*)szValue, nullInd, typeBind );
}

int GdbiStatement::Bind( int parmIndex, short *sValue, GDBI_NI_TYPE *nullInd, int typeBind)
{
	return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_SHORT,  sizeof(short), (char*)sValue, nullInd, typeBind );
}

int GdbiStatement::Bind( int parmIndex, int *nValue, GDBI_NI_TYPE *nullInd, int typeBind)
{
	return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_INT,  sizeof(int), (char*)nValue, nullInd, typeBind );
}

int GdbiStatement::Bind( int parmIndex, double *dValue, GDBI_NI_TYPE *nullInd, int typeBind)
{
    return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_DOUBLE,  sizeof(double), (char*)dValue, nullInd, typeBind );
}

int GdbiStatement::Bind( int parmIndex, float *fValue, GDBI_NI_TYPE *nullInd, int typeBind)
{
    return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_FLOAT,  sizeof(float), (char*)fValue, nullInd, typeBind );
}

int GdbiStatement::Bind( int parmIndex, FdoInt64 *i64Value, GDBI_NI_TYPE *nullInd, int typeBind)
{
    return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_LONGLONG,  sizeof(FdoInt64), (char*)i64Value, nullInd, typeBind );
}

int GdbiStatement::Bind( int parmIndex, FdoIGeometry* gValue, GDBI_NI_TYPE *nullInd, int typeBind)
{
    return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_GEOMETRY,  sizeof(FdoIGeometry), (char*)gValue, nullInd, typeBind );
}

int GdbiStatement::Bind(
        int parmIndex,
        int   datatype,     /* A data type from Inc/rdbi.h              */
        int   size,         /* binary size                              */
        char *address,      /* data address                             */
        GDBI_NI_TYPE *null_ind,     /* pointer to null indicator variables      */
        int typeBind)
{
    return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), datatype,  size, address, null_ind, typeBind );
}

int GdbiStatement::Bind(
        const char* parmName,
        int   datatype,     /* A data type from Inc/rdbi.h              */
        int   size,         /* binary size                              */
        char *address,      /* data address                             */
        GDBI_NI_TYPE *null_ind,     /* pointer to null indicator variables      */
        int typeBind)
{
    return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), (char*) parmName, datatype,  size, address, null_ind, typeBind);
}

int GdbiStatement::BindNull( int parmIndex )
{

    return 1;
}

int GdbiStatement::ParmCount( )
{

    return 1;
}

int GdbiStatement::ExecuteNonQuery( )
{
    if( m_pGdbiCommands->execute( m_QueryId->GetQueryId() ) == RDBI_SUCCESS )
    {
        return m_pGdbiCommands->crsr_nrows( m_QueryId->GetQueryId());
    }

    return 0;
}

GdbiQueryResult*  GdbiStatement::ExecuteQuery( )
{
	if( m_QueryId )
        return GdbiQueryResult::Create( m_pGdbiCommands, m_QueryId);

    throw new GdbiException(L"Not a valid query");
}

int GdbiStatement::Free()
{
	
	FDO_SAFE_RELEASE(m_QueryId);
	return RDBI_SUCCESS;
}

bool GdbiStatement::SupportsInt64Binding()
{ 
	if (m_QueryId)
        return (m_pGdbiCommands->SupportsInt64Binding());
	throw new GdbiException(L"Not a valid query");
}

int GdbiStatement::set_array_size(int arraySize)
{
	if (m_QueryId)	{
		m_pGdbiCommands->set_array_size(arraySize);
		return RDBI_SUCCESS;
	}
	throw new GdbiException(L"Not a valid query");
}

int GdbiStatement::geom_srid_set(
				int		parmIndex,
				long	srid
				)
{
	return m_pGdbiCommands->geom_srid_set(m_QueryId->GetQueryId(), 
		                                  FdoCommonOSUtil::itoa(parmIndex, buffer),
										  srid);
}

int GdbiStatement::geom_version_set(
				int		parmIndex,
				long	version
				)
{
	return m_pGdbiCommands->geom_version_set(m_QueryId->GetQueryId(), 
		                                  FdoCommonOSUtil::itoa(parmIndex, buffer),
										  version);
}

char * GdbiStatement::GetGeomInfoFromFgf( long srid, FdoByteArray * fgf )
{
#ifdef HAVE_GEOM_INFO_TYPE
    char * geomInfo = NULL;
    (void) m_pGdbiCommands->geom_from_fgf( m_QueryId->GetQueryId(), srid, (void *)fgf, (void**) &geomInfo );
    return geomInfo;
#else
    return NULL;
#endif
}

