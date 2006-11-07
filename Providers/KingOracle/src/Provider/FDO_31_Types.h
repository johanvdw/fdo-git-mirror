#pragma once
//  new 3.2 naming
#define D_FDO_3_1
#ifdef D_FDO_3_1

#define FdoPtr GisPtr
#define FDO_SAFE_RELEASE GIS_SAFE_RELEASE
#define FDO_SAFE_ADDREF GIS_SAFE_ADDREF

#define FdoIReader GisIReader
#define FdoByte GisByte
#define FdoByteArray GisByteArray
#define FdoString GisString
#define FdoStringP GisStringP
#define FdoStringCollection GisStringCollection
#define FdoInt16 GisInt16
#define FdoInt32 GisInt32
#define FdoInt64 GisInt64
#define FdoBoolean GisBoolean
#define FdoIoStream GisIoStream
#define FdoDateTime GisDateTime
#define FdoIStreamReader GisIStreamReader

#define FdoIPolygon GisIPolygon
#define FdoIGeometry GisIGeometry

#define FdoFgfGeometryFactory GisAgfGeometryFactory
#define CreateGeometryFromFgf CreateGeometryFromAgf
#define FdoIEnvelope GisIEnvelope
#define FdoEnvelopeImpl GisEnvelopeImpl
#define GetFgf GetAgf
#define FdoILinearRing GisILinearRing
#define FdoGeometryType_Point GisGeometryType_Point
#define FdoGeometryType_LineString GisGeometryType_LineString
#define FdoGeometryType_Polygon GisGeometryType_Polygon
#define FdoGeometryType_MultiPoint GisGeometryType_MultiPoint
#define FdoGeometryType_MultiLineString GisGeometryType_MultiLineString
#define FdoGeometryType_MultiPolygon GisGeometryType_MultiPolygon
#define FdoGeometryType_MultiGeometry GisGeometryType_MultiGeometry
#define FdoGeometryType_CurveString GisGeometryType_CurveString
#define FdoGeometryType_CurvePolygon GisGeometryType_CurvePolygon
#define FdoGeometryType_MultiCurveString GisGeometryType_MultiCurveString
#define FdoGeometryType_MultiCurvePolygon GisGeometryType_MultiCurvePolygon
#define FdoGeometryComponentType_LinearRing GisGeometryComponentType_LinearRing
#define FdoGeometryComponentType_LineStringSegment GisGeometryComponentType_LineStringSegment
#define FdoGeometryComponentType_Ring GisGeometryComponentType_Ring
#define FdoGeometryType GisGeometryType
#define FdoGeometryComponentType GisGeometryComponentType
#define FdoDimensionality_XY GisDimensionality_XY
#define FdoDimensionality_Z GisDimensionality_Z
#define FdoDimensionality_M GisDimensionality_M

#define FdoNamedCollection GisNamedCollection

#endif