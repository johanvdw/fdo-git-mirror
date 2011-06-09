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
 */

/************************************************************************
* Name																	*
*	odbcdr_define - Define the address and type of a select'ed column	*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	int odbcdr_define(cursor, name, datatype, size, address, null_ind)	*
*	char		*cursor;												*
*	char		*name;													*
*	int 		 datatype;												*
*	int 		 size;													*
*	char		*address;												*
*	short		*null_ind;												*
*																		*
* Description															*
*		Define	the  address,  size  and data type of a select'ed		*
*		column. 														*
*																		*
* Parameters															*
*	cursor: 	input													*
*		Pointer to the relevant cursor context area.  This cursor		*
*		must have had an SQL  statement  associated  with it by a		*
*		prior call to odbcdr_sql().	Also, that SQL statement must		*
*		be a 'select' statement.										*
*																		*
*	name:		input													*
*		Pointer  to  a	null  terminated ascii string identifying		*
*		which column variable to define.   Typcially strings like		*
*		"1"  or "5".  Eventually names like  "feat_num"  will  be		*
*		supported.	This is 1-based, not 0-based.						*
*																		*
*	datatype:	input													*
*		The  variable's  data  type.   eg.	 RDBI_STRING  --  see		*
*		inc/rdbi.h														*
*																		*
*	size:		input													*
*		The actual	binary size of the variable's data area.  Eg.		*
*		4 for long	integers.	In the case of character strings,		*
*		this length should be the maximum string length including		*
*		trailing null (or use the sizeof() construct).					*
*																		*
*		If	the  binary size of  a	variable  can  be  determined		*
*		directly from the  data  type,	this  parameter  will  be		*
*		ignored.														*
*																		*
*	address:	input	(output when odbcdr_fetch() is called)			*
*		The address of where to put the variable's data  --  cast		*
*		as a  pointer  to  char,  regardless  of  the actual data		*
*		type.															*
*																		*
*	null_ind:	input	(output when odbcdr_fetch() is called)			*
*		The  address  of  a  short	integer in which  to  put  an		*
*		indication as to whether the variable is NULL or not.  (0		*
*		==> not NULL; -1 ==> NULL).   If  this	pointer is itself		*
*		NULL,  the	 variable's   NULL	 status   will	 not   be		*
*		unequivocably detectable.										*
*																		*
* Function Value														*
*		An	RDBI  status  integer.	Good is RDBI_SUCCESS (ie  0).		*
*		See inc/rdbi.h.  If the name supplied is not found in the		*
*		select	 column  list,	RDBI_NOT_IN_DESC_LIST	will   be		*
*		returned.														*
*																		*
* Remarks																*
*		It is  quite  permissable  to  re-define  the same column		*
*		multiple  times.   The	last definition will be  the  one		*
*		used.															*
*																		*
************************************************************************/

#include <Inc/rdbi.h>			/* rdbi status values */
#include <Inc/ut.h> 				/* ut_vm_malloc() */
#include	<Inc/debugext.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

#include "proto_p.h"

 
int odbcdr_define(
    odbcdr_context_def *context,
	char	*cursor,
	char	*name,
	int 	 datatype,
	int 	 size,
	char	*address,
	SQLLEN	*null_ind
	)
{
	odbcdr_cursor_def	*c;
	int 				position;
	//int 				atoi();
	int 				odbcdr_datatype;
	int 				odbcdr_size;
	int 				rdbi_status = RDBI_GENERIC_ERROR;
	SQLRETURN			rc = ODBCDR_SUCCESS;
	int					is_numeric = TRUE;	/* no alphabetic name support */

	debug_on5("odbcdr_define","c:%#x name: %s type: %d address: 0x%lx size: %d",
							cursor, name, datatype, (long) address, size);

	ODBCDR_RDBI_ERR( odbcdr_get_cursor( context, cursor, &c ) );

	position = atoi(name);	/* Numeric positions are the only valid input	*/

	odbcdr_get_o_t_s( context, datatype, size, &odbcdr_datatype, &odbcdr_size );

    // Spetial handling for Geometries
    if ( datatype != RDBI_GEOMETRY )
	{
        if ( datatype != RDBI_BLOB_ULEN && datatype != RDBI_WSTRING_ULEN && datatype != RDBI_STRING_ULEN)
        {
            ODBCDR_ODBC_ERR( SQLBindCol( c->hStmt,
									(SQLUSMALLINT) position,
									(SQLSMALLINT) odbcdr_datatype,
									(SQLPOINTER) address,
									(SQLINTEGER) size,
									(SQLLEN *) null_ind),
							SQL_HANDLE_STMT, c->hStmt,
							"SQLBindCol", "define" );
        }
        else // RDBI_BLOB_ULEN , RDBI_WSTRING_ULEN , RDBI_STRING_ULEN
        {
            ODBCDR_RDBI_ERR( odbcdr_blob_defineColumn(context, c, position, address));

            ODBCDR_RDBI_ERR( odbcdr_blob_setNumRows(context, c, ODBCDR_MAX_ARRAY_SIZE));

            // Allocate the buffers for blobs and Null indicators
            int numBlobCols = c->defined_blobs->size;
            if ( numBlobCols == 1 )
            {
                c->odbcdr_blob = (PBYTE)malloc( ODBCDR_MAX_ARRAY_SIZE * ODBCDR_UBLOB_CHUNK_SIZE );
                c->odbcdr_blobNI = (SQLLEN *)malloc( ODBCDR_MAX_ARRAY_SIZE * sizeof(SQLLEN));
            }
            else
            {  
                c->odbcdr_blob = (PBYTE)realloc( c->odbcdr_blob, numBlobCols * ODBCDR_MAX_ARRAY_SIZE * ODBCDR_UBLOB_CHUNK_SIZE );
                c->odbcdr_blobNI = (SQLLEN *)realloc( c->odbcdr_blobNI, numBlobCols * ODBCDR_MAX_ARRAY_SIZE * sizeof(SQLLEN));
            }

            // Do binds (the previous bound columns will be rebound because of reallocation)
            for ( int i = 0; i < numBlobCols; i++ )
            {
                odbcdr_blob_col_def *column = (odbcdr_blob_col_def *) ut_da_get( c->defined_blobs, i );

                int offset = i * ODBCDR_MAX_ARRAY_SIZE;

	            ODBCDR_ODBC_ERR( SQLBindCol( c->hStmt,
								            (SQLUSMALLINT) column->position,
								            (SQLSMALLINT)  odbcdr_datatype,
                                            (SQLPOINTER)   (char *)&c->odbcdr_blob[offset * ODBCDR_UBLOB_CHUNK_SIZE],
                                            (SQLINTEGER)   ODBCDR_UBLOB_CHUNK_SIZE,
								            (SQLLEN *)     (char *)&c->odbcdr_blobNI[offset]),
						            SQL_HANDLE_STMT, c->hStmt,
						            "SQLBindCol", "define" );
            }
        }
	}
	else
	{
        ODBCDR_RDBI_ERR( odbcdr_geom_defineColumn(context, c, position, address));

        ODBCDR_RDBI_ERR( odbcdr_geom_setNumRows(context, c, ODBCDR_MAX_ARRAY_SIZE));

        // Allocate the buffers for Geometries and Null indicators
        int numGeomCols = c->defined_geometries->size;
        if ( numGeomCols == 1 )
        {
            c->odbcdr_geom = (PBYTE)malloc( ODBCDR_MAX_ARRAY_SIZE * ODBCDR_BLOB_CHUNK_SIZE );
            c->odbcdr_geomNI = (SQLLEN *)malloc( ODBCDR_MAX_ARRAY_SIZE * sizeof(SQLLEN));
        }
        else
        {  
            c->odbcdr_geom = (PBYTE)realloc( c->odbcdr_geom, numGeomCols * ODBCDR_MAX_ARRAY_SIZE * ODBCDR_BLOB_CHUNK_SIZE );
            c->odbcdr_geomNI = (SQLLEN *)realloc( c->odbcdr_geomNI, numGeomCols * ODBCDR_MAX_ARRAY_SIZE * sizeof(SQLLEN));
        }

        // Do binds (the previous bound columns will be rebound because of reallocation)
        for ( int i = 0; i < numGeomCols; i++ )
        {
            odbcdr_geom_col_def *column = (odbcdr_geom_col_def *) ut_da_get( c->defined_geometries, i );

            int offset = i * ODBCDR_MAX_ARRAY_SIZE;

	        ODBCDR_ODBC_ERR( SQLBindCol( c->hStmt,
								        (SQLUSMALLINT) column->position,
								        (SQLSMALLINT)  odbcdr_datatype,
                                        (SQLPOINTER)   (char *)&c->odbcdr_geom[offset * ODBCDR_BLOB_CHUNK_SIZE],
                                        (SQLINTEGER)   ODBCDR_BLOB_CHUNK_SIZE,
								        (SQLLEN *)     (char *)&c->odbcdr_geomNI[offset]),
						        SQL_HANDLE_STMT, c->hStmt,
						        "SQLBindCol", "define" );
        }

    }   /* end if datatype == RDBI_GEOMETRY */

	rdbi_status = RDBI_SUCCESS;

the_exit:
	debug1("Returning cursor c:%#x", cursor);
	debug_area() odbcdr_show_context( context, c );
	debug_return(NULL, rdbi_status );
}
