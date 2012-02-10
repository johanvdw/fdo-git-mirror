#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>

#define OTL_ODBC_MYSQL // Compile OTL 4/MyODBC
#include <otlv4.h> // include the OTL 4 header file

otl_connect db; // connect object

void insert()
// insert rows into table
{otl_long_string f2(6000); // define long string variable
 otl_stream o; // defined an otl_stream variable
 o.set_lob_stream_mode(true); // set the "lob stream mode" flag
 o.open(1, // buffer size has to be set to 1 for operations with LOBs
        "insert into test_tab values(:f1<int>,:f2<varchar_long>, "
        ":f3<varchar_long>) ",
            // SQL statement
        db // connect object
       );
 o.set_commit(0); // setting stream "auto-commit" to "off". It is required
                  // when LOB stream mode is used.

 int i,j;
 otl_lob_stream lob; // LOB stream for reading/writing unlimited number
                     // of bytes regardless of the buffer size.
 otl_lob_stream lob2; // LOB stream for reading/writing unlimited number
                     // of bytes regardless of the buffer size.

 for(i=1;i<=20;++i){
  o<<i;
  o<<lob; // Initialize otl_lob_stream by writing it
          // into otl_stream.
  o<<lob2; // Initialize otl_lob_stream by writing it
          // into otl_stream.

  for(j=0;j<5000;++j)
   f2[j]='*';
  f2[5000]='?';
  f2.set_len(5001);

  lob.set_len(5001+2123); // setting the total  size of
                          // the LONGTEXT to be written.
  
  lob<<f2; // writing first chunk of the LONGTEXT into lob


  f2[2122]='?';
  f2.set_len(2123); // setting the size of the second chunk

  lob<<f2; // writing the second chunk of the LONGTEXT into lob
  lob.close(); // closing the otl_lob_stream

  for(j=0;j<5000;++j)
   f2[j]='*';
  f2[5000]='?';
  f2.set_len(5001);
  lob2.set_len(5001+2123); // setting the total  size of
                          // the LONGTEXT to be written.
  
  lob2<<f2; // writing first chunk of the LONGTEXT into lob

  f2[2122]='?';
  f2.set_len(2123); // setting the size of the second chunk

  lob2<<f2; // writing the second chunk of the LONGTEXT into lob
  lob2.close(); // closing the otl_lob_stream

 }

 db.commit(); // committing transaction.
}

void update()
// insert rows in table
{

 otl_long_string f2(6200); // define long string variable

 otl_stream o; // defined an otl_stream variable
 o.set_lob_stream_mode(true); // set the "lob stream mode" flag
 o.open(1, // buffer size has to be set to 1 for operations with LOBs
              "update test_tab "
              "   set f2=:f2<varchar_long> "
              "where f1=:f1<int> ",
                 // SQL statement
              db // connect object
             );

  otl_lob_stream lob;

  o.set_commit(0); // setting stream "auto-commit" to "off". 


 for(int j=0;j<6000;++j){
  f2[j]='#';
 }

 f2[6000]='?';
 f2.set_len(6001);

 o<<lob; // Initialize otl_lob_stream by writing it
         // into otl_stream.
 o<<5;

 lob.set_len(6001*4); // setting the total size of of the LONGTEXT to be written
 for(int i=1;i<=4;++i)
  lob<<f2; // writing chunks of the LONGTEXT into the otl_lob_stream

 lob.close(); // closing the otl_lob_stream

 db.commit(); // committing transaction

}

void select()
{ 
 otl_long_string f2(3000); // define long string variable

 otl_stream i; // defined an otl_stream variable
 i.set_lob_stream_mode(true); // set the "lob stream mode" flag
 char stmbuf[1024];
 sprintf(stmbuf,
         "select * from test_tab where f1>=%d and f1<=%d*2",
          4,4
         );
 i.open(1, // buffer size. To read LONGTEXTs, it should be set to 1
        stmbuf, // SELECT statement
        db // connect object
       ); 
   // create select stream
 
 int f1;
 otl_lob_stream lob; // Stream for reading LONGTEXT
 otl_lob_stream lob2; // Stream for reading LONGTEXT

 while(!i.eof()){ // while not end-of-data
  i>>f1;
  cout<<"f1="<<f1<<endl;
  i>>lob; // initializing LOB stream by reading the LONGTEXT reference 
          // into the otl_lob_stream from the otl_stream.
  i>>lob2; // initializing LOB stream by reading the LONGTEXT reference 
          // into the otl_lob_stream from the otl_stream.
  int n=0;
  while(!lob.eof()){ // read while not "end-of-file" -- end of LONGTEXT
   ++n;
   lob>>f2; // reading a chunk of LONGTEXT
   cout<<"   chunk #"<<n;
   cout<<", f2="<<f2[0]<<f2[f2.len()-1]<<", len="<<f2.len()<<endl;
  }
  lob.close(); // closing the otl_lob_stream.
  n=0;
  while(!lob2.eof()){ // read while not "end-of-file" -- end of LONGTEXT
   ++n;
   lob2>>f2; // reading a chunk of LONGTEXT
   cout<<"   chunk #"<<n;
   cout<<", f3="<<f2[0]<<f2[f2.len()-1]<<", len="<<f2.len()<<endl;
  }
  lob2.close(); // closing the otl_lob_stream. 
  
 }

}

int main()
{
 otl_connect::otl_initialize(); // initialize the environment
 try{

  db.rlogon("scott/tiger@mysql"); // connect to the database

  otl_cursor::direct_exec
   (
    db,
    "drop table test_tab",
    otl_exception::disabled // disable OTL exceptions
   ); // drop table

  otl_cursor::direct_exec
   (
    db,
    "create table test_tab(f1 int, f2 longtext, f3 longtext)"
    );  // create table

  insert(); // insert records into table
  update(); // update records in table
  select(); // select records from table

 }

 catch(otl_exception& p){ // intercept OTL exceptions
  cerr<<p.msg<<endl; // print out error message
  cerr<<p.sqlstate<<endl; // print out SQLSTATE 
  cerr<<p.stm_text<<endl; // print out SQL that caused the error
  cerr<<p.var_info<<endl; // print out the variable that caused the error
 }

 db.logoff(); // disconnect from the database

 return 0;

}
