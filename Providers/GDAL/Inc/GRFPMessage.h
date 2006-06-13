#ifndef _H_GRFPMESSAGE_MSG 
#define _H_GRFPMESSAGE_MSG 
#define MF_GRFPMESSAGE GRFPMessage_cat



/* The following was generated from GRFPMessage.msf. */


/* definitions for set S_GRFPMESSAGE_1 */
#define S_GRFPMESSAGE_1 1

#define GRFP_1_UNEXPECTED_ERROR 1
#define GRFP_2_OPEN_FAILED 2
#define GRFP_3_NULL_ARGUMENT 3
#define GRFP_4_INVALID_PROPERTY_NAME 4
#define GRFP_5_COMMAND_TIMEOUT_NOT_SUPPORTED 5
#define GRFP_6_RELATION_DIRECTIVES_NOT_SUPPORTED 6
#define GRFP_7_MEMORY_FAILURE 7
#define GRFP_8_UNSUPPORTED_METADATA_VALUE 8
#define GRFP_9_CANT_OVERRIDE_IDENTITY_READONLY 9
#define GRFP_10_OBJECT_PROPERTIES_NOT_SUPPORTED 10
#define GRFP_11_FEATURE_CLASS_NOT_FOUND 11
#define GRFP_12_PROPERTY_NOT_IN_RESULT 12
#define GRFP_13_INDEX_OUT_OF_RANGE 13
#define GRFP_14_CLASS_NAME_EMPTY 14
#define GRFP_15_CONNECTION_NOT_ESTABLISHED 15
#define GRFP_16_VERSION_NULL 16
#define GRFP_17_READER_CLOSED 17
#define GRFP_18_READER_EXHAUSTED 18
#define GRFP_19_NOT_OWNER 19
#define GRFP_20_USER_UNKNOWN 20
#define GRFP_21_VERSION_INFO_ALLOC 21
#define GRFP_22_VERSION_INFO 22
#define GRFP_23_VERSION_INFO_ITEM 23
#define GRFP_24_VERSION_CREATE_FAILED 24
#define GRFP_25_VERSION_INFO_ITEM 25
#define GRFP_26_VERSION_INFO_LIST 26
#define GRFP_27_OFFSET_TOO_LARGE 27
#define GRFP_28_OFFSET_NEGATIVE 28
#define GRFP_29_INVALID_COUNT 29
#define GRFP_30_COMMAND_PARAMETERS_NOT_SUPPORTED 30
#define GRFP_31_FILTER_PARAMETERS_NOT_SUPPORTED 31
#define GRFP_32_CLOBS_NOT_SUPPORTED 32
#define GRFP_33_COULDNT_SET_VALUE 33
#define GRFP_34_INSERT_UNEXPECTED_ERROR 34
#define GRFP_35_VALUE_TYPE_MISMATCH 35
#define GRFP_36_NO_ACTIVE_TRANSACTION 36
#define GRFP_37_CONNECTION_INVALID 37
#define GRFP_38_CANNOT_SET_CONNECTION_STRING 38
#define GRFP_39_CONNECTION_TIMEOUT_NOT_SUPPORTED 39
#define GRFP_40_CONNECTION_ALREADY_OPEN 40
#define GRFP_41_INVALID_CONNECTION_CONFIGURATION 41
#define GRFP_42_TRANSACTION_NOT_SUPPORTED 42
#define GRFP_43_CANNOT_SET_CONFIGURATION 43
#define GRFP_44_INVALID_FILTER 44
#define GRFP_45_COMMAND_NOT_SUPPORTED 45
#define GRFP_46_CLASS_INVALID 46
#define GRFP_47_PROPERTY_TYPE_NOT_SUPPORTED 47
#define GRFP_48_FEATURE_SCHEMA_NOT_FOUND 48
#define GRFP_49_CLASS_NAME_MUST_BE_SPECIFIED 49
#define GRFP_52_ONLY_MOSAIC_CLIP_RESAMPLE_SUPPORTED 50
#define GRFP_53_PROPERTY_NOT_EXIST 51
#define GRFP_55_MOSAIC_CLIP_RESAMPLE_ONLY_APPLY_TO_RASTER 52
#define GRFP_56_ILLEGAL_REQUESTED_PROPERTY 53
#define GRFP_57_READNEXT_MUST_BE_CALLED 54
#define GRFP_58_TYPE_MISMATCH 55
#define GRFP_59_OPERATION_NOT_SUPPORTED 56
#define GRFP_60_SPATIAL_CONTEXT_NOT_EXIST 57
#define GRFP_61_SPATIAL_CONTEXT_EXIST 58
#define GRFP_62_INVALID_IMAGE_DEF 59
#define GRFP_63_FAIL_LOAD_RASTER 60
#define GRFP_64_RASTER_NULL 61
#define GRFP_65_DATAMODEL_NOT_SUPPORTED 62
#define GRFP_66_DEFAULT_SPATIAL_CONTEXT_NAME 63
#define GRFP_67_DEFAULT_SPATIAL_CONTEXT_DESC 64
#define GRFP_68_PROPERTY_NOT_DEFINED 65
#define GRFP_69_CONNECTION_ESTABLISHED 66
#define GRFP_70_DEFAULT_RASTER_FILE_LOCATION 67
#define GRFP_72_LOCK_NOT_SUPPORTED 68
#define GRFP_73_UNSUPPORTED_TILE_SIZE 69
#define GRFP_74_RASTER_EXCEPTION 70
#define GRFP_75_SCHEMA_MAPPING_NOT_FOUND 71
#define GRFP_76_FAIL_READ_SPATIAL_CONTEXTS 72
#define GRFP_77_FAIL_READ_FEATURE_SCHEMAS 73
#define GRFP_78_FAIL_READ_SCHEMA_MAPPINGS 74
#define GRFP_79_RASTER_PROPERTY_NOT_EXIST 75
#define GRFP_80_INDEX_OUT_OF_RANGE 76
#define GRFP_81_RASTER_ARGUMENT_DESC 77
#define GRFP_82_MOSAIC_FUNCTION_DESC 78
#define GRFP_84_CLIP_INCORRECTLY_USED 79
#define GRFP_85_MOSAIC_INCORRECTLY_USED 80
#define GRFP_86_DOUBLE_ARGUMENT_DESC 81
#define GRFP_87_CLIP_FUNCTION_DESC 82
#define GRFP_88_CAN_NOT_SELECT_AGGREGATE 83
#define GRFP_89_UNEXPECTED_RASTER_EXCEPTION 84
#define GRFP_90_RASTER_TOO_LARGE 85
#define GRFP_91_INVALID_GEOREF 86
#define GRFP_92_BAND_NUMBER_NOT_SEQUENTIAL 87
#define GRFP_93_DATAMODEL_ILLEGALLY_STITCHED 88
#define GRFP_94_INVALID_GEOREFERENCE_DEF 89
#define GRFP_95_CANNOT_GET_IMAGE_INFO 90
#define GRFP_96_FRAME_NUMBER_OUT_OF_RANGE 91
#define GRFP_97_MULTI_COORD_SYSTEM_FOR_SINGLE_FEATRUE_CLASS 92
#define GRFP_98_MULTI_SPATIAL_CONTEXTS_NOT_SUPPORTED 93
#define GRFP_99_INVALID_XML_ATTRIBUTE 94
#define GRFP_100_NO_GEOREFERENCE 95
#define GRFP_101_FEATURE_TAG_NOT_FOUND_UNDER_LOCATION 96
#define GRFP_102_BAND_TAG_NOT_FOUND_UNDER_FEATURE 97
#define GRFP_103_IMAGE_TAG_NOT_FOUND_UNDER_BAND 98
#define GRFP_104_LOCATION_TAG_NOT_FOUND_UNDER_RASTERDEFINITION 99
#define GRFP_105_RESAMPLE_INCORRECTLY_USED 100
#define GRFP_106_RESAMPLE_FUNCTION_DESC 101
#define GRFP_107_UINT_ARGUMENT_DESC 102
#define GRFP_108_CLASS_SPATIAL_CONTEXT_NOT_DEFINED 103
#endif 
