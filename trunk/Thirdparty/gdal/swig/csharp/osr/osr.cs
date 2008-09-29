/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.31
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace OSGeo.OSR {

using System;
using System.Runtime.InteropServices;

public class osr {
  public static void UseExceptions() {
    osrPINVOKE.UseExceptions();
  }

  public static void DontUseExceptions() {
    osrPINVOKE.DontUseExceptions();
  }

  public static int GetWellKnownGeogCSAsWKT(string name, out string argout) {
    int ret = osrPINVOKE.GetWellKnownGeogCSAsWKT(name, out argout);
    if (osrPINVOKE.SWIGPendingException.Pending) throw osrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int GetUserInputAsWKT(string name, out string argout) {
    int ret = osrPINVOKE.GetUserInputAsWKT(name, out argout);
    if (osrPINVOKE.SWIGPendingException.Pending) throw osrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static SWIGTYPE_p_p_char GetProjectionMethods() {
    IntPtr cPtr = osrPINVOKE.GetProjectionMethods();
    SWIGTYPE_p_p_char ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_p_char(cPtr, false? null : new object());
    if (osrPINVOKE.SWIGPendingException.Pending) throw osrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static SWIGTYPE_p_p_char GetProjectionMethodParameterList(string method, out string username) {
    IntPtr cPtr = osrPINVOKE.GetProjectionMethodParameterList(method, out username);
    SWIGTYPE_p_p_char ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_p_char(cPtr, false? null : new object());
    if (osrPINVOKE.SWIGPendingException.Pending) throw osrPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static void GetProjectionMethodParamInfo(string method, string param, out string usrname, out string type, ref double defaultval) {
    osrPINVOKE.GetProjectionMethodParamInfo(method, param, out usrname, out type, ref defaultval);
    if (osrPINVOKE.SWIGPendingException.Pending) throw osrPINVOKE.SWIGPendingException.Retrieve();
  }

  public static readonly string SRS_PT_ALBERS_CONIC_EQUAL_AREA = osrPINVOKE.SRS_PT_ALBERS_CONIC_EQUAL_AREA_get();
  public static readonly string SRS_PT_AZIMUTHAL_EQUIDISTANT = osrPINVOKE.SRS_PT_AZIMUTHAL_EQUIDISTANT_get();
  public static readonly string SRS_PT_CASSINI_SOLDNER = osrPINVOKE.SRS_PT_CASSINI_SOLDNER_get();
  public static readonly string SRS_PT_CYLINDRICAL_EQUAL_AREA = osrPINVOKE.SRS_PT_CYLINDRICAL_EQUAL_AREA_get();
  public static readonly string SRS_PT_ECKERT_IV = osrPINVOKE.SRS_PT_ECKERT_IV_get();
  public static readonly string SRS_PT_ECKERT_VI = osrPINVOKE.SRS_PT_ECKERT_VI_get();
  public static readonly string SRS_PT_EQUIDISTANT_CONIC = osrPINVOKE.SRS_PT_EQUIDISTANT_CONIC_get();
  public static readonly string SRS_PT_EQUIRECTANGULAR = osrPINVOKE.SRS_PT_EQUIRECTANGULAR_get();
  public static readonly string SRS_PT_GALL_STEREOGRAPHIC = osrPINVOKE.SRS_PT_GALL_STEREOGRAPHIC_get();
  public static readonly string SRS_PT_GNOMONIC = osrPINVOKE.SRS_PT_GNOMONIC_get();
  public static readonly string SRS_PT_GOODE_HOMOLOSINE = osrPINVOKE.SRS_PT_GOODE_HOMOLOSINE_get();
  public static readonly string SRS_PT_HOTINE_OBLIQUE_MERCATOR = osrPINVOKE.SRS_PT_HOTINE_OBLIQUE_MERCATOR_get();
  public static readonly string SRS_PT_HOTINE_OBLIQUE_MERCATOR_TWO_POINT_NATURAL_ORIGIN = osrPINVOKE.SRS_PT_HOTINE_OBLIQUE_MERCATOR_TWO_POINT_NATURAL_ORIGIN_get();
  public static readonly string SRS_PT_LABORDE_OBLIQUE_MERCATOR = osrPINVOKE.SRS_PT_LABORDE_OBLIQUE_MERCATOR_get();
  public static readonly string SRS_PT_LAMBERT_CONFORMAL_CONIC_1SP = osrPINVOKE.SRS_PT_LAMBERT_CONFORMAL_CONIC_1SP_get();
  public static readonly string SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP = osrPINVOKE.SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP_get();
  public static readonly string SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP_BELGIUM = osrPINVOKE.SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP_BELGIUM_get();
  public static readonly string SRS_PT_LAMBERT_AZIMUTHAL_EQUAL_AREA = osrPINVOKE.SRS_PT_LAMBERT_AZIMUTHAL_EQUAL_AREA_get();
  public static readonly string SRS_PT_MERCATOR_1SP = osrPINVOKE.SRS_PT_MERCATOR_1SP_get();
  public static readonly string SRS_PT_MERCATOR_2SP = osrPINVOKE.SRS_PT_MERCATOR_2SP_get();
  public static readonly string SRS_PT_MILLER_CYLINDRICAL = osrPINVOKE.SRS_PT_MILLER_CYLINDRICAL_get();
  public static readonly string SRS_PT_MOLLWEIDE = osrPINVOKE.SRS_PT_MOLLWEIDE_get();
  public static readonly string SRS_PT_NEW_ZEALAND_MAP_GRID = osrPINVOKE.SRS_PT_NEW_ZEALAND_MAP_GRID_get();
  public static readonly string SRS_PT_OBLIQUE_STEREOGRAPHIC = osrPINVOKE.SRS_PT_OBLIQUE_STEREOGRAPHIC_get();
  public static readonly string SRS_PT_ORTHOGRAPHIC = osrPINVOKE.SRS_PT_ORTHOGRAPHIC_get();
  public static readonly string SRS_PT_POLAR_STEREOGRAPHIC = osrPINVOKE.SRS_PT_POLAR_STEREOGRAPHIC_get();
  public static readonly string SRS_PT_POLYCONIC = osrPINVOKE.SRS_PT_POLYCONIC_get();
  public static readonly string SRS_PT_ROBINSON = osrPINVOKE.SRS_PT_ROBINSON_get();
  public static readonly string SRS_PT_SINUSOIDAL = osrPINVOKE.SRS_PT_SINUSOIDAL_get();
  public static readonly string SRS_PT_STEREOGRAPHIC = osrPINVOKE.SRS_PT_STEREOGRAPHIC_get();
  public static readonly string SRS_PT_SWISS_OBLIQUE_CYLINDRICAL = osrPINVOKE.SRS_PT_SWISS_OBLIQUE_CYLINDRICAL_get();
  public static readonly string SRS_PT_TRANSVERSE_MERCATOR = osrPINVOKE.SRS_PT_TRANSVERSE_MERCATOR_get();
  public static readonly string SRS_PT_TRANSVERSE_MERCATOR_SOUTH_ORIENTED = osrPINVOKE.SRS_PT_TRANSVERSE_MERCATOR_SOUTH_ORIENTED_get();
  public static readonly string SRS_PT_TRANSVERSE_MERCATOR_MI_22 = osrPINVOKE.SRS_PT_TRANSVERSE_MERCATOR_MI_22_get();
  public static readonly string SRS_PT_TRANSVERSE_MERCATOR_MI_23 = osrPINVOKE.SRS_PT_TRANSVERSE_MERCATOR_MI_23_get();
  public static readonly string SRS_PT_TRANSVERSE_MERCATOR_MI_24 = osrPINVOKE.SRS_PT_TRANSVERSE_MERCATOR_MI_24_get();
  public static readonly string SRS_PT_TRANSVERSE_MERCATOR_MI_25 = osrPINVOKE.SRS_PT_TRANSVERSE_MERCATOR_MI_25_get();
  public static readonly string SRS_PT_TUNISIA_MINING_GRID = osrPINVOKE.SRS_PT_TUNISIA_MINING_GRID_get();
  public static readonly string SRS_PT_VANDERGRINTEN = osrPINVOKE.SRS_PT_VANDERGRINTEN_get();
  public static readonly string SRS_PT_KROVAK = osrPINVOKE.SRS_PT_KROVAK_get();
  public static readonly string SRS_PP_CENTRAL_MERIDIAN = osrPINVOKE.SRS_PP_CENTRAL_MERIDIAN_get();
  public static readonly string SRS_PP_SCALE_FACTOR = osrPINVOKE.SRS_PP_SCALE_FACTOR_get();
  public static readonly string SRS_PP_STANDARD_PARALLEL_1 = osrPINVOKE.SRS_PP_STANDARD_PARALLEL_1_get();
  public static readonly string SRS_PP_STANDARD_PARALLEL_2 = osrPINVOKE.SRS_PP_STANDARD_PARALLEL_2_get();
  public static readonly string SRS_PP_PSEUDO_STD_PARALLEL_1 = osrPINVOKE.SRS_PP_PSEUDO_STD_PARALLEL_1_get();
  public static readonly string SRS_PP_LONGITUDE_OF_CENTER = osrPINVOKE.SRS_PP_LONGITUDE_OF_CENTER_get();
  public static readonly string SRS_PP_LATITUDE_OF_CENTER = osrPINVOKE.SRS_PP_LATITUDE_OF_CENTER_get();
  public static readonly string SRS_PP_LONGITUDE_OF_ORIGIN = osrPINVOKE.SRS_PP_LONGITUDE_OF_ORIGIN_get();
  public static readonly string SRS_PP_LATITUDE_OF_ORIGIN = osrPINVOKE.SRS_PP_LATITUDE_OF_ORIGIN_get();
  public static readonly string SRS_PP_FALSE_EASTING = osrPINVOKE.SRS_PP_FALSE_EASTING_get();
  public static readonly string SRS_PP_FALSE_NORTHING = osrPINVOKE.SRS_PP_FALSE_NORTHING_get();
  public static readonly string SRS_PP_AZIMUTH = osrPINVOKE.SRS_PP_AZIMUTH_get();
  public static readonly string SRS_PP_LONGITUDE_OF_POINT_1 = osrPINVOKE.SRS_PP_LONGITUDE_OF_POINT_1_get();
  public static readonly string SRS_PP_LATITUDE_OF_POINT_1 = osrPINVOKE.SRS_PP_LATITUDE_OF_POINT_1_get();
  public static readonly string SRS_PP_LONGITUDE_OF_POINT_2 = osrPINVOKE.SRS_PP_LONGITUDE_OF_POINT_2_get();
  public static readonly string SRS_PP_LATITUDE_OF_POINT_2 = osrPINVOKE.SRS_PP_LATITUDE_OF_POINT_2_get();
  public static readonly string SRS_PP_LONGITUDE_OF_POINT_3 = osrPINVOKE.SRS_PP_LONGITUDE_OF_POINT_3_get();
  public static readonly string SRS_PP_LATITUDE_OF_POINT_3 = osrPINVOKE.SRS_PP_LATITUDE_OF_POINT_3_get();
  public static readonly string SRS_PP_RECTIFIED_GRID_ANGLE = osrPINVOKE.SRS_PP_RECTIFIED_GRID_ANGLE_get();
  public static readonly string SRS_PP_LANDSAT_NUMBER = osrPINVOKE.SRS_PP_LANDSAT_NUMBER_get();
  public static readonly string SRS_PP_PATH_NUMBER = osrPINVOKE.SRS_PP_PATH_NUMBER_get();
  public static readonly string SRS_PP_PERSPECTIVE_POINT_HEIGHT = osrPINVOKE.SRS_PP_PERSPECTIVE_POINT_HEIGHT_get();
  public static readonly string SRS_PP_FIPSZONE = osrPINVOKE.SRS_PP_FIPSZONE_get();
  public static readonly string SRS_PP_ZONE = osrPINVOKE.SRS_PP_ZONE_get();
  public static readonly string SRS_UL_METER = osrPINVOKE.SRS_UL_METER_get();
  public static readonly string SRS_UL_FOOT = osrPINVOKE.SRS_UL_FOOT_get();
  public static readonly string SRS_UL_FOOT_CONV = osrPINVOKE.SRS_UL_FOOT_CONV_get();
  public static readonly string SRS_UL_US_FOOT = osrPINVOKE.SRS_UL_US_FOOT_get();
  public static readonly string SRS_UL_US_FOOT_CONV = osrPINVOKE.SRS_UL_US_FOOT_CONV_get();
  public static readonly string SRS_UL_NAUTICAL_MILE = osrPINVOKE.SRS_UL_NAUTICAL_MILE_get();
  public static readonly string SRS_UL_NAUTICAL_MILE_CONV = osrPINVOKE.SRS_UL_NAUTICAL_MILE_CONV_get();
  public static readonly string SRS_UL_LINK = osrPINVOKE.SRS_UL_LINK_get();
  public static readonly string SRS_UL_LINK_CONV = osrPINVOKE.SRS_UL_LINK_CONV_get();
  public static readonly string SRS_UL_CHAIN = osrPINVOKE.SRS_UL_CHAIN_get();
  public static readonly string SRS_UL_CHAIN_CONV = osrPINVOKE.SRS_UL_CHAIN_CONV_get();
  public static readonly string SRS_UL_ROD = osrPINVOKE.SRS_UL_ROD_get();
  public static readonly string SRS_UL_ROD_CONV = osrPINVOKE.SRS_UL_ROD_CONV_get();
  public static readonly string SRS_DN_NAD27 = osrPINVOKE.SRS_DN_NAD27_get();
  public static readonly string SRS_DN_NAD83 = osrPINVOKE.SRS_DN_NAD83_get();
  public static readonly string SRS_DN_WGS72 = osrPINVOKE.SRS_DN_WGS72_get();
  public static readonly string SRS_DN_WGS84 = osrPINVOKE.SRS_DN_WGS84_get();
  public static readonly double SRS_WGS84_SEMIMAJOR = osrPINVOKE.SRS_WGS84_SEMIMAJOR_get();
  public static readonly double SRS_WGS84_INVFLATTENING = osrPINVOKE.SRS_WGS84_INVFLATTENING_get();
}

}
