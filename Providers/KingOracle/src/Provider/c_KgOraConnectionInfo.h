/*
* Copyright (C) 2006  SL-King d.o.o
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

#ifndef _c_KgOraConnectionInfo_h
#define _c_KgOraConnectionInfo_h


/// \brief
/// The FdoIConnectionInfo interface exposes information about the feature provider
/// and provides access to the FdoIConnectionPropertyDictionary interface.
class c_KgOraConnectionInfo : public FdoIConnectionInfo
{
    

public:
    c_KgOraConnectionInfo (c_KgOraConnection * m_Connection); 
    c_KgOraConnectionInfo (); 
    
    
protected:
    
    c_KgOraConnection * m_Connection;
    FdoPtr<FdoCommonConnPropDictionary> m_PropertyDictionary; // connection parameters        

protected:
    
    virtual void Dispose ();
   

public:
    /// <summary>Gets the name of the feature provider.</summary>
    /// <returns>Returns the provider name</returns>
    FDOKGORA_API FdoString* GetProviderName ();

    /// <summary>Gets the display name of the feature provider.</summary>
    /// <returns>Returns the provider's display name</returns>
    FDOKGORA_API FdoString* GetProviderDisplayName();

    /// <summary>Gets the description of the feature provider.</summary>
    /// <returns>Returns the provider description</returns>
    FDOKGORA_API FdoString* GetProviderDescription ();

    /// <summary>Gets the version of the feature provider.</summary>
    /// <returns>Returns provider version</returns>
    FDOKGORA_API FdoString* GetProviderVersion ();

    /// <summary>Gets the version of the Feature Data Objects specification to which this provider conforms.</summary>
    /// <returns>Returns FDO version supported.</returns>
    FDOKGORA_API FdoString* GetFeatureDataObjectsVersion ();

    /// <summary>Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.</summary>
    /// <returns>Returns the property dictionary</returns>
    FDOKGORA_API FdoIConnectionPropertyDictionary* GetConnectionProperties ();

};

#endif 

