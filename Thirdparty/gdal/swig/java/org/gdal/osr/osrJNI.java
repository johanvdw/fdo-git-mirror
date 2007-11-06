/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.31
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.gdal.osr;

class osrJNI {
  public final static native String SRS_PT_ALBERS_CONIC_EQUAL_AREA_get();
  public final static native String SRS_PT_AZIMUTHAL_EQUIDISTANT_get();
  public final static native String SRS_PT_CASSINI_SOLDNER_get();
  public final static native String SRS_PT_CYLINDRICAL_EQUAL_AREA_get();
  public final static native String SRS_PT_ECKERT_IV_get();
  public final static native String SRS_PT_ECKERT_VI_get();
  public final static native String SRS_PT_EQUIDISTANT_CONIC_get();
  public final static native String SRS_PT_EQUIRECTANGULAR_get();
  public final static native String SRS_PT_GALL_STEREOGRAPHIC_get();
  public final static native String SRS_PT_GNOMONIC_get();
  public final static native String SRS_PT_GOODE_HOMOLOSINE_get();
  public final static native String SRS_PT_HOTINE_OBLIQUE_MERCATOR_get();
  public final static native String SRS_PT_HOTINE_OBLIQUE_MERCATOR_TWO_POINT_NATURAL_ORIGIN_get();
  public final static native String SRS_PT_LABORDE_OBLIQUE_MERCATOR_get();
  public final static native String SRS_PT_LAMBERT_CONFORMAL_CONIC_1SP_get();
  public final static native String SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP_get();
  public final static native String SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP_BELGIUM_get();
  public final static native String SRS_PT_LAMBERT_AZIMUTHAL_EQUAL_AREA_get();
  public final static native String SRS_PT_MERCATOR_1SP_get();
  public final static native String SRS_PT_MERCATOR_2SP_get();
  public final static native String SRS_PT_MILLER_CYLINDRICAL_get();
  public final static native String SRS_PT_MOLLWEIDE_get();
  public final static native String SRS_PT_NEW_ZEALAND_MAP_GRID_get();
  public final static native String SRS_PT_OBLIQUE_STEREOGRAPHIC_get();
  public final static native String SRS_PT_ORTHOGRAPHIC_get();
  public final static native String SRS_PT_POLAR_STEREOGRAPHIC_get();
  public final static native String SRS_PT_POLYCONIC_get();
  public final static native String SRS_PT_ROBINSON_get();
  public final static native String SRS_PT_SINUSOIDAL_get();
  public final static native String SRS_PT_STEREOGRAPHIC_get();
  public final static native String SRS_PT_SWISS_OBLIQUE_CYLINDRICAL_get();
  public final static native String SRS_PT_TRANSVERSE_MERCATOR_get();
  public final static native String SRS_PT_TRANSVERSE_MERCATOR_SOUTH_ORIENTED_get();
  public final static native String SRS_PT_TRANSVERSE_MERCATOR_MI_22_get();
  public final static native String SRS_PT_TRANSVERSE_MERCATOR_MI_23_get();
  public final static native String SRS_PT_TRANSVERSE_MERCATOR_MI_24_get();
  public final static native String SRS_PT_TRANSVERSE_MERCATOR_MI_25_get();
  public final static native String SRS_PT_TUNISIA_MINING_GRID_get();
  public final static native String SRS_PT_VANDERGRINTEN_get();
  public final static native String SRS_PT_KROVAK_get();
  public final static native String SRS_PP_CENTRAL_MERIDIAN_get();
  public final static native String SRS_PP_SCALE_FACTOR_get();
  public final static native String SRS_PP_STANDARD_PARALLEL_1_get();
  public final static native String SRS_PP_STANDARD_PARALLEL_2_get();
  public final static native String SRS_PP_PSEUDO_STD_PARALLEL_1_get();
  public final static native String SRS_PP_LONGITUDE_OF_CENTER_get();
  public final static native String SRS_PP_LATITUDE_OF_CENTER_get();
  public final static native String SRS_PP_LONGITUDE_OF_ORIGIN_get();
  public final static native String SRS_PP_LATITUDE_OF_ORIGIN_get();
  public final static native String SRS_PP_FALSE_EASTING_get();
  public final static native String SRS_PP_FALSE_NORTHING_get();
  public final static native String SRS_PP_AZIMUTH_get();
  public final static native String SRS_PP_LONGITUDE_OF_POINT_1_get();
  public final static native String SRS_PP_LATITUDE_OF_POINT_1_get();
  public final static native String SRS_PP_LONGITUDE_OF_POINT_2_get();
  public final static native String SRS_PP_LATITUDE_OF_POINT_2_get();
  public final static native String SRS_PP_LONGITUDE_OF_POINT_3_get();
  public final static native String SRS_PP_LATITUDE_OF_POINT_3_get();
  public final static native String SRS_PP_RECTIFIED_GRID_ANGLE_get();
  public final static native String SRS_PP_LANDSAT_NUMBER_get();
  public final static native String SRS_PP_PATH_NUMBER_get();
  public final static native String SRS_PP_PERSPECTIVE_POINT_HEIGHT_get();
  public final static native String SRS_PP_FIPSZONE_get();
  public final static native String SRS_PP_ZONE_get();
  public final static native String SRS_UL_METER_get();
  public final static native String SRS_UL_FOOT_get();
  public final static native String SRS_UL_FOOT_CONV_get();
  public final static native String SRS_UL_US_FOOT_get();
  public final static native String SRS_UL_US_FOOT_CONV_get();
  public final static native String SRS_UL_NAUTICAL_MILE_get();
  public final static native String SRS_UL_NAUTICAL_MILE_CONV_get();
  public final static native String SRS_UL_LINK_get();
  public final static native String SRS_UL_LINK_CONV_get();
  public final static native String SRS_UL_CHAIN_get();
  public final static native String SRS_UL_CHAIN_CONV_get();
  public final static native String SRS_UL_ROD_get();
  public final static native String SRS_UL_ROD_CONV_get();
  public final static native String SRS_DN_NAD27_get();
  public final static native String SRS_DN_NAD83_get();
  public final static native String SRS_DN_WGS72_get();
  public final static native String SRS_DN_WGS84_get();
  public final static native double SRS_WGS84_SEMIMAJOR_get();
  public final static native double SRS_WGS84_INVFLATTENING_get();

  private static boolean available = false;

  static {
    try {
      System.loadLibrary("osrjni");
      available = true;
    } catch (UnsatisfiedLinkError e) {
      available = false;
      System.err.println("Native library load failed.");
      System.err.println(e);
    }
  }
  
  public static boolean isAvailable() {
    return available;
  }

  public final static native int GetWellKnownGeogCSAsWKT(String jarg1, String[] jarg2);
  public final static native int GetUserInputAsWKT(String jarg1, String[] jarg2);
  public final static native String[] GetProjectionMethods();
  public final static native String[] GetProjectionMethodParameterList(String jarg1, String[] jarg2);
  public final static native void GetProjectionMethodParamInfo(String jarg1, String jarg2, String[] jarg3, String[] jarg4, long jarg5);
  public final static native long new_SpatialReference(String jarg1);
  public final static native void delete_SpatialReference(long jarg1);
  public final static native String SpatialReference___str__(long jarg1, SpatialReference jarg1_);
  public final static native int SpatialReference_IsSame(long jarg1, SpatialReference jarg1_, long jarg2, SpatialReference jarg2_);
  public final static native int SpatialReference_IsSameGeogCS(long jarg1, SpatialReference jarg1_, long jarg2, SpatialReference jarg2_);
  public final static native int SpatialReference_IsGeographic(long jarg1, SpatialReference jarg1_);
  public final static native int SpatialReference_IsProjected(long jarg1, SpatialReference jarg1_);
  public final static native int SpatialReference_IsLocal(long jarg1, SpatialReference jarg1_);
  public final static native String SpatialReference_GetAttrValue(long jarg1, SpatialReference jarg1_, String jarg2, int jarg3);
  public final static native int SpatialReference_SetAttrValue(long jarg1, SpatialReference jarg1_, String jarg2, String jarg3);
  public final static native int SpatialReference_SetAngularUnits(long jarg1, SpatialReference jarg1_, String jarg2, double jarg3);
  public final static native double SpatialReference_GetAngularUnits(long jarg1, SpatialReference jarg1_);
  public final static native int SpatialReference_SetLinearUnits(long jarg1, SpatialReference jarg1_, String jarg2, double jarg3);
  public final static native double SpatialReference_GetLinearUnits(long jarg1, SpatialReference jarg1_);
  public final static native String SpatialReference_GetLinearUnitsName(long jarg1, SpatialReference jarg1_);
  public final static native String SpatialReference_GetAuthorityCode(long jarg1, SpatialReference jarg1_, String jarg2);
  public final static native String SpatialReference_GetAuthorityName(long jarg1, SpatialReference jarg1_, String jarg2);
  public final static native int SpatialReference_SetUTM(long jarg1, SpatialReference jarg1_, int jarg2, int jarg3);
  public final static native int SpatialReference_SetStatePlane(long jarg1, SpatialReference jarg1_, int jarg2, int jarg3, String jarg4, double jarg5);
  public final static native int SpatialReference_AutoIdentifyEPSG(long jarg1, SpatialReference jarg1_);
  public final static native int SpatialReference_SetProjection(long jarg1, SpatialReference jarg1_, String jarg2);
  public final static native int SpatialReference_SetProjParm(long jarg1, SpatialReference jarg1_, String jarg2, double jarg3);
  public final static native double SpatialReference_GetProjParm(long jarg1, SpatialReference jarg1_, String jarg2, double jarg3);
  public final static native int SpatialReference_SetNormProjParm(long jarg1, SpatialReference jarg1_, String jarg2, double jarg3);
  public final static native double SpatialReference_GetNormProjParm(long jarg1, SpatialReference jarg1_, String jarg2, double jarg3);
  public final static native int SpatialReference_SetACEA(long jarg1, SpatialReference jarg1_, double jarg2, double jarg3, double jarg4, double jarg5, double jarg6, double jarg7);
  public final static native int SpatialReference_SetAE(long jarg1, SpatialReference jarg1_, double jarg2, double jarg3, double jarg4, double jarg5);
  public final static native int SpatialReference_SetCS(long jarg1, SpatialReference jarg1_, double jarg2, double jarg3, double jarg4, double jarg5);
  public final static native int SpatialReference_SetBonne(long jarg1, SpatialReference jarg1_, double jarg2, double jarg3, double jarg4, double jarg5);
  public final static native int SpatialReference_SetEC(long jarg1, SpatialReference jarg1_, double jarg2, double jarg3, double jarg4, double jarg5, double jarg6, double jarg7);
  public final static native int SpatialReference_SetEckertIV(long jarg1, SpatialReference jarg1_, double jarg2, double jarg3, double jarg4);
  public final static native int SpatialReference_SetEckertVI(long jarg1, SpatialReference jarg1_, double jarg2, double jarg3, double jarg4);
  public final static native int SpatialReference_SetEquirectangular(long jarg1, SpatialReference jarg1_, double jarg2, double jarg3, double jarg4, double jarg5);
  public final static native int SpatialReference_SetGS(long jarg1, SpatialReference jarg1_, double jarg2, double jarg3, double jarg4);
  public final static native int SpatialReference_SetWellKnownGeogCS(long jarg1, SpatialReference jarg1_, String jarg2);
  public final static native int SpatialReference_SetFromUserInput(long jarg1, SpatialReference jarg1_, String jarg2);
  public final static native int SpatialReference_CopyGeogCSFrom(long jarg1, SpatialReference jarg1_, long jarg2, SpatialReference jarg2_);
  public final static native int SpatialReference_SetTOWGS84(long jarg1, SpatialReference jarg1_, double jarg2, double jarg3, double jarg4, double jarg5, double jarg6, double jarg7, double jarg8);
  public final static native int SpatialReference_GetTOWGS84(long jarg1, SpatialReference jarg1_, double[] jarg2);
  public final static native int SpatialReference_SetGeogCS(long jarg1, SpatialReference jarg1_, String jarg2, String jarg3, String jarg4, double jarg5, double jarg6, String jarg7, double jarg8, String jarg9, double jarg10);
  public final static native int SpatialReference_SetProjCS(long jarg1, SpatialReference jarg1_, String jarg2);
  public final static native int SpatialReference_ImportFromWkt(long jarg1, SpatialReference jarg1_, String jarg2);
  public final static native int SpatialReference_ImportFromProj4(long jarg1, SpatialReference jarg1_, String jarg2);
  public final static native int SpatialReference_ImportFromESRI(long jarg1, SpatialReference jarg1_, String jarg2);
  public final static native int SpatialReference_ImportFromEPSG(long jarg1, SpatialReference jarg1_, int jarg2);
  public final static native int SpatialReference_ImportFromPCI(long jarg1, SpatialReference jarg1_, String jarg2, String jarg3, double[] jarg4);
  public final static native int SpatialReference_ImportFromUSGS(long jarg1, SpatialReference jarg1_, int jarg2, int jarg3, double[] jarg4, int jarg5);
  public final static native int SpatialReference_ImportFromXML(long jarg1, SpatialReference jarg1_, String jarg2);
  public final static native int SpatialReference_ExportToWkt(long jarg1, SpatialReference jarg1_, String[] jarg2);
  public final static native int SpatialReference_ExportToPrettyWkt(long jarg1, SpatialReference jarg1_, String[] jarg2, int jarg3);
  public final static native int SpatialReference_ExportToProj4(long jarg1, SpatialReference jarg1_, String[] jarg2);
  public final static native int SpatialReference_ExportToPCI(long jarg1, SpatialReference jarg1_, String[] jarg2, String[] jarg3, double[] jarg4);
  public final static native int SpatialReference_ExportToUSGS(long jarg1, SpatialReference jarg1_, int[] jarg2, int[] jarg3, double[] jarg4, int[] jarg5);
  public final static native int SpatialReference_ExportToXML(long jarg1, SpatialReference jarg1_, String[] jarg2, String jarg3);
  public final static native long SpatialReference_CloneGeogCS(long jarg1, SpatialReference jarg1_);
  public final static native int SpatialReference_Validate(long jarg1, SpatialReference jarg1_);
  public final static native int SpatialReference_StripCTParms(long jarg1, SpatialReference jarg1_);
  public final static native int SpatialReference_FixupOrdering(long jarg1, SpatialReference jarg1_);
  public final static native int SpatialReference_Fixup(long jarg1, SpatialReference jarg1_);
  public final static native int SpatialReference_MorphToESRI(long jarg1, SpatialReference jarg1_);
  public final static native int SpatialReference_MorphFromESRI(long jarg1, SpatialReference jarg1_);
  public final static native long new_CoordinateTransformation(long jarg1, SpatialReference jarg1_, long jarg2, SpatialReference jarg2_);
  public final static native void delete_CoordinateTransformation(long jarg1);
  public final static native void CoordinateTransformation_TransformPoint__SWIG_0(long jarg1, CoordinateTransformation jarg1_, double[] jarg2);
  public final static native void CoordinateTransformation_TransformPoint__SWIG_1(long jarg1, CoordinateTransformation jarg1_, double[] jarg2, double jarg3, double jarg4, double jarg5);
}
