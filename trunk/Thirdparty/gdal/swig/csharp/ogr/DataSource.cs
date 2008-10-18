/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.31
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace OSGeo.OGR {

using System;
using System.Runtime.InteropServices;

public class DataSource : IDisposable {
  private HandleRef swigCPtr;
  protected object swigCMemOwner;

  internal DataSource(IntPtr cPtr, object cMemoryOwner) {
    swigCMemOwner = cMemoryOwner;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(DataSource obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
  internal static HandleRef getCPtrAndDisown(DataSource obj, object cMemoryOwner) {
    obj.swigCMemOwner = cMemoryOwner;
    return getCPtr(obj);
  }

  ~DataSource() {
    Dispose();
  }

  public virtual void Dispose() {
  lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwner == null) {
        swigCMemOwner = new object();
        ogrPINVOKE.delete_DataSource(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public string name {
    get {
      string ret = ogrPINVOKE.DataSource_name_get(swigCPtr);
      if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  public int GetRefCount() {
    int ret = ogrPINVOKE.DataSource_GetRefCount(swigCPtr);
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int GetSummaryRefCount() {
    int ret = ogrPINVOKE.DataSource_GetSummaryRefCount(swigCPtr);
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int GetLayerCount() {
    int ret = ogrPINVOKE.DataSource_GetLayerCount(swigCPtr);
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Driver GetDriver() {
    IntPtr cPtr = ogrPINVOKE.DataSource_GetDriver(swigCPtr);
    Driver ret = (cPtr == IntPtr.Zero) ? null : new Driver(cPtr, false? null : this);
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public string GetName() {
    string ret = ogrPINVOKE.DataSource_GetName(swigCPtr);
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int DeleteLayer(int index) {
    int ret = ogrPINVOKE.DataSource_DeleteLayer(swigCPtr, index);
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Layer CreateLayer(string name, SpatialReference reference, int geom_type, string[] options) {
    IntPtr cPtr = ogrPINVOKE.DataSource_CreateLayer(swigCPtr, name, SpatialReference.getCPtr(reference), geom_type, new ogrPINVOKE.StringListMarshal(options)._ar);
    Layer ret = (cPtr == IntPtr.Zero) ? null : new Layer(cPtr, false? null : this);
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Layer CopyLayer(Layer src_layer, string new_name, string[] options) {
    IntPtr cPtr = ogrPINVOKE.DataSource_CopyLayer(swigCPtr, Layer.getCPtr(src_layer), new_name, new ogrPINVOKE.StringListMarshal(options)._ar);
    Layer ret = (cPtr == IntPtr.Zero) ? null : new Layer(cPtr, false? null : this);
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Layer GetLayerByIndex(int index) {
    IntPtr cPtr = ogrPINVOKE.DataSource_GetLayerByIndex(swigCPtr, index);
    Layer ret = (cPtr == IntPtr.Zero) ? null : new Layer(cPtr, false? null : this);
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Layer GetLayerByName(string layer_name) {
    IntPtr cPtr = ogrPINVOKE.DataSource_GetLayerByName(swigCPtr, layer_name);
    Layer ret = (cPtr == IntPtr.Zero) ? null : new Layer(cPtr, false? null : this);
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public bool TestCapability(string cap) {
    bool ret = ogrPINVOKE.DataSource_TestCapability(swigCPtr, cap);
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Layer ExecuteSQL(string statement, Geometry geom, string dialect) {
    IntPtr cPtr = ogrPINVOKE.DataSource_ExecuteSQL(swigCPtr, statement, Geometry.getCPtr(geom), dialect);
    Layer ret = (cPtr == IntPtr.Zero) ? null : new Layer(cPtr, true? null : this);
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void ReleaseResultSet(Layer layer) {
    ogrPINVOKE.DataSource_ReleaseResultSet(swigCPtr, Layer.getCPtrAndDisown(layer, this));
    if (ogrPINVOKE.SWIGPendingException.Pending) throw ogrPINVOKE.SWIGPendingException.Retrieve();
  }

}

}
