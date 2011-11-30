#ifndef _GDBICOMMANDS_
#define _GDBICOMMANDS_ 1
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
#ifdef _WIN32
#pragma once
#endif

#include "GdbiQueryResult.h"
#include "GdbiException.h"
#ifdef USE_NONRDBMS_HEADER
#include <inc/rdbi/context.h>
#include <inc/rdbi/vndr_info.h>
#else
#include <Inc/Rdbi/context.h>
#include <Inc/Rdbi/vndr_info.h>
#endif


#define DBI_FEATURE_SEQUENCE    "feature"
#define DBI_LOCK_SEQUENCE       "lock"
#define DBI_VERSION_SEQUENCE    "version"
#define DBI_PLAN_SEQUENCE       "plan"
#define DBI_USER_SEQUENCE       "user"
#define DBI_CLASS_SEQUENCE      "class"
#define DBI_GENERATION_SEQUENCE "generation"
#define DBI_PLAN_GROUP_SEQUENCE "plan_group"

#define DBI_FEATURE_SEQUENCEW    L"feature"
#define DBI_LOCK_SEQUENCEW       L"lock"
#define DBI_VERSION_SEQUENCEW    L"version"
#define DBI_PLAN_SEQUENCEW       L"plan"
#define DBI_USER_SEQUENCEW       L"user"
#define DBI_CLASS_SEQUENCEW      L"class"
#define DBI_GENERATION_SEQUENCEW L"generation"
#define DBI_PLAN_GROUP_SEQUENCEW L"plan_group"

/* sequences */
#define ADB_FEATURESEQ                      "F_FEATURESEQ"
#define ADB_LOCKSEQ                         "F_LOCKSEQ"
#define ADB_VERSIONSEQ                      "F_VERSIONSEQ"
#define ADB_PLANSEQ                         "F_PLANSEQ"
#define ADB_CLASSSEQ                        "F_CLASSSEQ"
#define ADB_PLANGROUPSEQ                    "F_PLANGROUPSEQ"
#define ADB_GENERATIONSEQ                   "F_GENERATIONSEQ"
#define ADB_USERSEQ                         "F_USERSEQ"

#define ADB_SN_ALLOC_INCREMENT              20
#define ADB_SEQUENCE_TABLE                  "f_sequence"
#define ADB_SQL_LEN                         1024
#define ADB_SEQID_COLUMN                    "seqid"
#define ADB_USERSEQ                         "F_USERSEQ"

#define ADB_CLASSDEF_TAB					"f_classdefinition"
#define ADB_SPATIAL_CONTEXT_TABLE   		"f_spatialcontext"
#define ADB_SPATIAL_CONTEXT_GROUP_TABLE		"f_spatialcontextgroup"

#define ADB_FEATURESEQW                      L"F_FEATURESEQ"
#define ADB_LOCKSEQW                         L"F_LOCKSEQ"
#define ADB_VERSIONSEQW                      L"F_VERSIONSEQ"
#define ADB_PLANSEQW                         L"F_PLANSEQ"
#define ADB_CLASSSEQW                        L"F_CLASSSEQ"
#define ADB_PLANGROUPSEQW                    L"F_PLANGROUPSEQ"
#define ADB_GENERATIONSEQW                   L"F_GENERATIONSEQ"
#define ADB_USERSEQW                         L"F_USERSEQ"

#define ADB_SEQUENCE_TABLEW                  L"f_sequence"
#define ADB_SEQID_COLUMNW                    L"seqid"
#define ADB_USERSEQW                         L"F_USERSEQ"

#define ADB_CLASSDEF_TABW					L"f_classdefinition"
#define ADB_SPATIAL_CONTEXT_TABLEW   		L"f_spatialcontext"
#define ADB_SPATIAL_CONTEXT_GROUP_TABLEW	L"f_spatialcontextgroup"

#ifndef _WIN32
#define _stricmp strcasecmp
#endif

#define	ut_utf8_from_unicode( p1,p2,p3 ) FdoStringP::Utf8FromUnicode( p1,p2,(int)p3, false )
#define	ut_utf8_to_unicode( p1,p2,p3 ) FdoStringP::Utf8ToUnicode( p1,p2,(int)p3, false )


typedef struct {
    long    next_seq;
    long    max_seq;
    long    seq_inc;
} gdbi_seq_def;

typedef struct {
    int     cursor;
    wchar_t seq_name[RDBI_TABLE_NAME_SIZE];
    int     next;
    int     size;
    long    sequence[ADB_SN_ALLOC_INCREMENT];
} gdbi_full_seq_def;

class GdbiCommands
{
    friend class GdbiQueryResult;
    friend class GdbiConnection;
private:
    rdbi_context_def* m_pRdbiContext;


    gdbi_full_seq_def    mFeatureSeq;

	int mArraySize;

    int desc_slct(
        int   sqlid,        /* Index into ptrs to Oracle work areas */
        int   pos,          /* position within select clause        */
        int   name_len,     /* The maximum size of name             */
        wchar_t *name,         /* Name of this field                   */
        int  *rdbi_type,    /* rdbi constant                        */
        int  *binary_size,  /* bytes of memory to store the field   */
        int  *null_ok );

    int end_select( int     sqlid );

    int fetch(
        int  sqlid,                    /* index into cursor globals    */
        int  count,                    /* # rows to fetch              */
        int *rows_processed );

    void CheckDB();

    void ThrowException();

    //
    // Use the RDBMS sequence number capability(i.e Oracle )
    long NextRDBMSSequenceNumber( FdoString*sequence );

    // In case sequence is not supported but autoincrement is, 
    // the method gets the current generated number
    FdoInt64 NextRDBMSAutoincrementNumber( FdoString*sequence );

    // In case sequence is not supported and also the native autoincrement, 
    // the method simulates a sequence allocation scheme.
    FdoInt64 NextGDBISequenceNumber( FdoString*sequence );
    
public:
    GdbiCommands( rdbi_context_def* rdbi_context );
    ~GdbiCommands(void);

    FdoInt64 GetLastSequenceNumber();

    // List of rdbi wrapper functions:
	int run_sql( FdoStringP sql,  bool isDDL, int *rows_processed = NULL  );

	int sql( FdoStringP sql,  int *qid  );

    int sql( FdoStringP sql,  int type, int *qid  );

    int execute( int qid  ) { return execute( qid, 1, 0); }

    int execute( int qid, int count, int offset );

    int free_cursor( int qid  );

    void set_null( GDBI_NI_TYPE *null_ind, int start, int end);

    void set_nnull( GDBI_NI_TYPE *null_ind, int start, int end);

    int is_null( GDBI_NI_TYPE *null_ind, int offset );

	int alcnullind( int n, GDBI_NI_TYPE **null_ind);

    int bind(
        int   cursorId,        /* index into cursor array                  */
        char *name,         /* column/expression position               */
        int   datatype,     /* A data type from Inc/rdbi.h              */
        int   size,         /* binary size                              */
        char *address,      /* data address                             */
        GDBI_NI_TYPE *null_ind,
        int typeBind = 1
    );

    int tran_begin( char    *tran_id );

    int tran_end( char    *tran_id );

    int tran_rolbk( );

    int sp_add(FdoStringP sp);
    int sp_rollback(FdoStringP sp);
    int sp_release(FdoStringP sp);
    bool sp_exists(FdoStringP sp);

	int	autocommit_on();
	int autocommit_off();
	int autocommit_mode();

    int define(
        int   sqlid,        /* index into cursor array                  */
        char *name,         /* column/expression position               */
        int   datatype,     /* A data type from Inc/rdbi.h              */
        int   size,         /* binary size                              */
        char *address,      /* data address                             */
        GDBI_NI_TYPE *null_ind /* pointer to null indicator variables      */
    );

    int vndr_info(
        rdbi_vndr_info_def *info
    );

    //
    // Number of processed rows
    int crsr_nrows(  int sqlid );

    FdoInt64 NextSequenceNumber( FdoString* sequence );

    // Returns true if insert/update is allowed (the autogenerated columns may be not)
	bool IsFieldWriteable( FdoStringP tableName, FdoStringP columnName );

	int err_stat();

	bool SupportsUnicode() { 
        return (m_pRdbiContext->dispatch.capabilities.supports_unicode == 1); 
    }
	bool SupportsInt64Binding() { return (m_pRdbiContext->dispatch.capabilities.supports_int64_binding == 1); }


	void set_array_size(int arraySize);
	int  get_array_size();
	// this is relevant for Oracle only
	int lob_create_ref(int sqlid, void **lob_ref);
	int lob_destroy_ref(int sqlid, void *lob_ref);
    int lob_get_size(int sqlid, void *lob_ref, unsigned int *size);
    int lob_read_next(int sqlid, void *lob_ref, int rdbi_lob_type, unsigned int block_size, char *block, unsigned int *block_size_out, int *eol);

    int geom_to_fgf ( 
            int             sqlid,
            void *          rdbmsGeometryInfo_I,
            int             defaultDim,
            void **         fgfGeometryByteArray_O );

    int geom_from_fgf ( 
            int             sqlid,
            long            srid,
            void *          fgfGeometryByteArray_I,
            void **         rdbmsGeometryInfo_O );

	int geom_srid_set(
			int				sqlid,
			char			*geom_col_name,
			long			srid );

	int geom_version_set(
			int				sqlid,
			char			*geom_col_name,
			long			version );
};

#endif // _GDBICOMMANDS_
