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
*/

#ifndef TESTCOMMONMISCUTIL___H
#define TESTCOMMONMISCUTIL___H    1

#ifdef _WIN32
#pragma once
#endif

namespace FdoCommonUtilTypes
{
    template<class T1, class T2> struct pair
    {
        pair()
            : first(T1()), second(T2())
        {}
        pair(const T1& val1, const T2& val2)
            : first(val1), second(val2)
        {}
        T1 first;
        T2 second;
    };
    template <class T> class vector
    {
    private:
        size_t m_size;
        size_t m_capacity;
        T* m_objects;
    private:
        void resize()
        {
            size_t oldCap = m_capacity;
            m_capacity = (m_capacity == 0) ? 8 : (size_t)(m_capacity * 1.4);
            T* objects = new T[m_capacity];
            for (size_t i = 0; i < m_capacity; i++)
            {
                if (i < oldCap)
                    objects[i] = m_objects[i];
                else
                    objects[i] = T();
            }
            delete[] m_objects;
            m_objects = objects;
        }
    public:
        vector()
        {
            m_size = m_capacity = 0;
            m_objects = NULL;
        }
        ~vector()
        {
            delete[] m_objects;
        }
        size_t size() { return m_size; }
        void push_back(const T& val)
        {
            if ((m_size+1) >= m_capacity)
                resize();
            m_objects[m_size] = val;
            m_size++;
        }
        T& operator[](size_t pos)
        {
            if (pos < m_size)
                return m_objects[pos];
            throw FdoException::Create(L"Index out of bounds!");
        }
        T& at(size_t pos)
        {
            if (pos < m_size)
                return m_objects[pos];
            throw FdoException::Create(L"Index out of bounds!");
        }
        void clear()
        {
            m_size = 0;
        }
        void pop_back()
        {
            if (m_size > 0)
                m_size--;
        }
    };
};
//
// Misc commonly-used utility functions:
void TestCommonFail (FdoException* ge);
void TestCommonFail (char* error);

//
// Utilities common to many unit tests
class TestCommonMiscUtil
{
public:
    // Retrieve the named property value from a property value collection.
    // Create and add the property value to the collection if it is not already there.
    static FdoPropertyValue* AddNewProperty( FdoPropertyValueCollection* propertyValues, FdoString* name );

    // Convenience function for adding an object to an FDO class definition.
    //      connection - object is inserted into the datastore for this connection
    //      insertCmd - the insert command for inserting the object. If NULL, this function
    //          will create its own insert command. However, when the caller inserts multiple objects,
    //          it can create one insert command and re-use it by passing it to each call to InsertObject.
    //      schemaName - schema containing object's class
    //      className - name of object's class
    //      ... - 1 or more property values. 3 arguments per property value to insert:
    //          FdoString* - property name 
    //          FdoDataType - property type
    //          FdoDataValue - property value. Specific type must match property type
    //      Last property value must be following by (FdoString*) NULL to indicate the end of the 
    //      property list.
    static FdoPtr<FdoIInsert> InsertObject( FdoIConnection* connection, FdoPtr<FdoIInsert> insertCmd, FdoStringP schemaName, FdoString* className, ... );

    // Convenience function for deleting objects from an FDO class definition.
    //      connection - objects are deleted from the datastore for this connection
    //      schemaName - schema containing objects' class
    //      className - name of objects' class
    //      ... optional filter property value
    //          FdoString* - property name (if NULL then all objects in the class are deleted ).
    //          FdoDataType - property type
    //          FdoDataValue - property value. All objects whose given property has this value are deleted.
    static void DeleteObjects( FdoIConnection* connection, FdoStringP schemaName, FdoString* className, ... );

    static bool GetSpatialContext( 
        FdoIConnection* connection,
        FdoStringP name, 
        FdoStringP& description,
        FdoStringP& coordSys,
        FdoStringP& wkt,
        FdoPtr<FdoByteArray>& extent,
        FdoSpatialContextExtentType& extentType,
        double& xyTol,
        double& zTol
    );

    static void ByteArray2Extent( FdoByteArray* ba, double& minx, double& miny, double& maxx, double& maxy );

    static void VldExtent( FdoStringP scName, FdoByteArray* ba, double minx, double miny, double maxx, double maxy );

    static FdoClassDefinition* DescribeClass( FdoIConnection* fdoConn, FdoStringP schemaName, FdoStringP className, FdoFeatureSchemasP& parentHolder );

#ifdef _WIN32
    // Quick and dirty function to use in Visual Studio to report memory leaks. This can be done by
    // adding a call to SetupLeakReport to a unit test. The report lists the allocation number for 
    // each piece of leaked memory. For leaked FDO objects, the refcount is also reported. If the object
    // is an FdoSchemaElement then the element name is reported.
    // Feel free to add additional information for FDO object when needed.
    //
    // There is currently a bug in that a segv is generated while writing the report. However, 
    // the more interesting memory leaks have usually been reported at this point.
    static void SetupLeakReport();
#endif

    // Get time on system clock (in seconds) -- used for computing elapsed time.
    static double GetTime_S(void);

    // check that 2 doubles are roughly equal:
    static bool FuzzyEqual (const double d1, const double d2);

    // Trim trailing blanks from a string.
    static FdoStringP Trim( FdoStringP in );

protected:

    static FdoPtr<FdoLiteralValue> ArgsToLiteral( va_list& arguments );

private:
#ifdef _WIN32
    // Memory Leak reporting hook that is set up by SetupLeakReport.
    static int LeakReportHook( int reportType, char *message, int *returnValue );
#endif
};

#endif // TESTCOMMONMISCUTIL___H


