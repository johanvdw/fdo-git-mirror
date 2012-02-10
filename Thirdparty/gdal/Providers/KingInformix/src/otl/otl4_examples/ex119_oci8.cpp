#include <iostream>
using namespace std;
#include <string.h>

#if defined(_MSC_VER) 
#if (_MSC_VER >= 1400)
#pragma warning (disable:4996)
#endif
#endif

class super_fast_string{
public:

  super_fast_string(void)
  {
    buf[0]=0;
  }

  super_fast_string(const char* s)
  {
    strcpy(buf,s);
  }

  super_fast_string(const super_fast_string& s)
  {
    strcpy(buf,s.buf);
  }

  ~super_fast_string(){}

  super_fast_string& operator=(const char* s)
  {
    strcpy(buf,s);
    return *this;
  }

  super_fast_string& operator=(const super_fast_string& s)
  {
    strcpy(buf,s.buf);
    return *this;
  }

  super_fast_string& operator+=(const super_fast_string& s)
  {
    strcat(buf,s.buf);
    return *this;
  }

  const char* c_str(void) const
  {
    return buf;
  }

  void assign(char* temp_buf,int len)
  {
    for(int i=0;i<len;++i)
      buf[i]=temp_buf[i];
    buf[len]=0;
  }

  char& operator[](int ndx)
  {
    return buf[ndx];
  }

  size_t length(void) const 
  {
    return strlen(buf);
  }


protected:

   char buf[512];

};

ostream& operator<<(ostream& s,const super_fast_string& str)
{
  s<<str.c_str();
  return s;
}

#define OTL_ORA8I // Compile OTL 4/OCI8
#define OTL_VALUE_TEMPLATE_ON // Turn on otl_value<T>
#define OTL_USER_DEFINED_STRING_CLASS_ON
#define USER_DEFINED_STRING_CLASS super_fast_string
#include <otlv4.h> // include the OTL 4 header file

// Uncomment the line below when OCI7 is used with OTL
// #define OTL_ORA7 // Compile OTL 4.0/OCI7 

otl_connect db; // connect object

void insert()
// insert rows into table
{ 
 otl_stream o(50, // buffer size
              "insert into test_tab "
              "values(:f1<int>,:f2<char[31]>,:f3<timestamp>)", 
                 // SQL statement
              db // connect object
             );

 otl_value<super_fast_string> f2; // otl_value container with a string class
 otl_value<otl_datetime> f3; // container of otl_datetime


 for(int i=1;i<=100;++i){

  if(i%2==0)
   f2="NameXXX";
  else
   f2=otl_null(); // Assign otl_null() to f2

  if(i%3==0){
   // Assign a value to f3 via the .v field directly
   f3.v.year=2001;
   f3.v.month=1;
   f3.v.day=1;
   f3.v.hour=13;
   f3.v.minute=10;
   f3.v.second=5;
   f3.set_non_null(); // Set f3 as a "non-NULL"
  }else
   f3.set_null(); // Set f3 as null via .set_null() function

  o<<i<<f2<<f3;

 }
}

void select()
{ 
 otl_stream i(50, // buffer size
              "select * from test_tab where f1>=:f11<int> and f1<=:f12<int>*2",
                 // SELECT statement
              db // connect object
             ); 
   // create select stream

 int f1; 
 otl_value<super_fast_string> f2;
 otl_value<otl_datetime> f3;


 i<<8<<8; // assigning :f11 = :f12 = 8

   // SELECT automatically executes when all input variables are
   // assigned. First portion of output rows is fetched to the buffer

 while(!i.eof()){ // while not end-of-data
  i>>f1>>f2>>f3;
  cout<<"f1="<<f1<<", f2="<<f2<<", f3="<<f3<<endl;
 }

 i<<4<<4; // assigning :f11 = :f12 = 4
   // SELECT automatically executes when all input variables are
   // assigned. First portion of output rows is fetched to the buffer

 while(!i.eof()){ // while not end-of-data
  i>>f1>>f2>>f3;
  cout<<"f1="<<f1<<", f2="<<f2<<", f3="<<f3<<endl;
 }

}

int main()
{
 otl_connect::otl_initialize(); // initialize the database API environment
 try{

  db.rlogon("scott/tiger"); // connect to teh database

  otl_cursor::direct_exec
   (
    db,
    "drop table test_tab",
    otl_exception::disabled // disable OTL exceptions
   ); // drop table

  otl_cursor::direct_exec
   (
    db,
    "create table test_tab(f1 int, f2 varchar(30), f3 date)"
    );  // create table

  insert(); // insert records into table
  select(); // select records from table

 }

 catch(otl_exception& p){ // intercept OTL exceptions
  cerr<<p.msg<<endl; // print out error message
  cerr<<p.sqlstate<<endl; // print out SQLSTATE
  cerr<<p.stm_text<<endl; // print out SQL that caused the error
  cerr<<p.var_info<<endl; // print out the variable that caused the error
 }

 db.logoff(); // disconnect from teh database

 return 0;

}
