// 
//  
//  Copyright (C) 2004-2005  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "SQLiteSqlUpdateCache.h"
#include "SQLiteMemoryDataBase.h"
#include "SQLiteTable.h"
#include "SQLiteBTree.h"
#include "SQLiteData.h"
#include "SQLiteCursor.h"

SQLiteSqlUpdateCache::SQLiteSqlUpdateCache(SQLiteDataBase *db, unsigned int size, int rootPage, 
										   unsigned int nextKey, bool useCompression, bool useIntKey,
										   SQLiteBTreeCompareHandler *hdl ) 
{ 
    mCur = NULL;
    m_pTargetDb = db;
    m_nextKey = nextKey;
    mRootPage = rootPage;
    mUseCompression = useCompression;
    mUseIntKey = useIntKey;
	mCmpHandler = hdl;
    m_pDb = new SQLiteMemoryDataBase( );

    if( m_pDb->begin_transaction() != SQLITE_OK )
        return;

    int flags = (mUseIntKey)?BTREE_INTKEY:0;
    if( m_pDb->BTree()->create_table( flags, &mTabId ) )
        return;

    if( m_pDb->BTree()->cursor( mTabId, &mCur, 1, mCmpHandler ) )
        return;
}

SQLiteSqlUpdateCache::~SQLiteSqlUpdateCache() 
{ 
    if( m_pDb )
        delete m_pDb;

    if (mCur)
        delete mCur;
}

bool SQLiteSqlUpdateCache::CanPut(SQLiteData *key)
{
    return true;
}

int SQLiteSqlUpdateCache::put(SQLiteTransaction *txid, SQLiteData *key, SQLiteData *data, unsigned int flags) 
{ 
    if( key->get_size() == 0 )
    {
        // new record
        mMykey = m_nextKey++;
        key->set_size( sizeof(SQLiteRecNumbDef) );
        key->set_data(&mMykey );
    }
    else if( key->get_size() == 4 )  // Only valid for autogenerated int properties
    {
        int nextKey = (*((int*)key->get_data()));
         if( m_nextKey == nextKey )
             m_nextKey++;
        if( m_nextKey < nextKey )
            m_nextKey = nextKey + 1;
    }
    return mCur->insert( key->get_size(), (unsigned char*)key->get_data(), data->get_size(), (unsigned char*)data->get_data(), false);
}

int SQLiteSqlUpdateCache::get(SQLiteTransaction *txnid, SQLiteData *key, SQLiteData *data, unsigned int flags) 
{ 
        bool found;
        int rc = mCur->move_to( key->get_size(), (unsigned char*)key->get_data(), found );

        if( !found )
            rc = SQLiteDB_NOTFOUND;

        char* buf = NULL;
        int dat_size=0;

        if( rc == 0 && mCur->get_data( &dat_size, &buf, false ) )
            rc = 1;

        if( rc == 0 )
        {
            data->set_size( dat_size );
            data->set_data( (void*)buf );
        }
        return rc;
}

int SQLiteSqlUpdateCache::del( SQLiteData *key )
{
      bool found;
      int rc = mCur->move_to( key->get_size(), (unsigned char*)key->get_data(), found );

      if( !found )
        return SQLiteDB_NOTFOUND;

      rc = mCur->delete_current();

      return rc;
}

int SQLiteSqlUpdateCache::flush( ) 
{
    int rc;
	bool needsCommit = false;
    SQLiteCursor *pmCur = NULL;

    m_pTargetDb->close_all_read_cursors();
    if( m_pTargetDb->BTree()->cursor( mRootPage, &pmCur, 1, mCmpHandler ) )
        return SQLITE_ERROR;

	if( ! m_pTargetDb->transaction_started() )
	{
		if( m_pTargetDb->begin_transaction( ) != SQLITE_OK )
		{
			(void) pmCur->close();
			delete pmCur;
			return SQLITE_ERROR;
		}
		else
			needsCommit = true;
	}
   
    if( m_pDb->commit() != SQLITE_OK )
        return SQLITE_ERROR;

    if( mCur )
    {
        mCur->close(); // Close the write cursor
        delete mCur;
    }

    if( m_pDb->BTree()->cursor( mTabId, &mCur, 0, mCmpHandler ) != SQLITE_OK ) // Open a read-only cursor
        return SQLITE_ERROR;

    if( mCur->first( ) != SQLITE_OK ) // Empty?
	{
		if( needsCommit )
		 (void)m_pTargetDb->commit( );

		pmCur->close();
		delete pmCur;

		mCur->close(); // Close the read cursor and open a write cursor
		delete mCur;
		mCur = NULL;

		// Start a write transaction again before leaving
		if( m_pDb->begin_transaction() != SQLITE_OK )
			return SQLITE_ERROR;

		if( m_pDb->BTree()->cursor( mTabId, &mCur, 1, mCmpHandler ) )
			return SQLITE_ERROR;

        return SQLITE_OK;
	}
    do
    {
        char* keyBuf = NULL;
        char* datBuf = NULL;
        int dat_size=0;
        int key_size;

        if( mCur->get_key( &key_size, &keyBuf ) )
            return SQLITE_ERROR;

        if( mCur->get_data( &dat_size, &datBuf, false ) )
            return SQLITE_ERROR;

         rc = pmCur->insert( key_size, (unsigned char*)keyBuf, dat_size, (unsigned char*)(datBuf), mUseCompression );
        _ASSERT( rc==SQLITE_OK );
        if( rc != SQLITE_OK )
        {
            return SQLITE_ERROR;
        }
      
    } while ( mCur->next( ) == SQLITE_OK );

    rc = pmCur->close();
    delete pmCur;
     _ASSERT( rc==SQLITE_OK );

	 if( needsCommit )
		 (void)m_pTargetDb->commit( );

    mCur->close(); // Close the read-only cursor
    delete mCur;
    delete m_pDb;

   m_pDb = new SQLiteMemoryDataBase( );
    
    if( m_pDb->begin_transaction() != SQLITE_OK )
        return SQLITE_ERROR;

    int flags = (mUseIntKey)?BTREE_INTKEY:0;
    if( m_pDb->BTree()->create_table( flags, &mTabId ) )
        return SQLITE_ERROR;

    if( m_pDb->BTree()->cursor( mTabId, &mCur, 1, mCmpHandler ) )
        return SQLITE_ERROR;

    return SQLITE_OK;
}

