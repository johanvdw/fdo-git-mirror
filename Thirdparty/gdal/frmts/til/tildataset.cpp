/******************************************************************************
 * $Id: tildataset.cpp 17664 2009-09-21 21:16:45Z rouault $
 *
 * Project:  EarthWatch .TIL Driver
 * Purpose:  Implementation of the TILDataset class.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 2009, Frank Warmerdam
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ****************************************************************************/

#include "gdal_pam.h"
#include "gdal_proxy.h"
#include "ogr_spatialref.h"
#include "cpl_string.h"
#include "vrtdataset.h"
#include "cpl_multiproc.h"
#include "cplkeywordparser.h"

CPL_CVSID("$Id: tildataset.cpp 17664 2009-09-21 21:16:45Z rouault $");

/************************************************************************/
/* ==================================================================== */
/*				TILDataset				*/
/* ==================================================================== */
/************************************************************************/

class CPL_DLL TILDataset : public GDALPamDataset
{
    VRTDataset *poVRTDS;
    std::vector<GDALDataset *> apoTileDS;

  public:
    TILDataset();
    ~TILDataset();

    static GDALDataset *Open( GDALOpenInfo * );
    static int Identify( GDALOpenInfo *poOpenInfo );
};

/************************************************************************/
/* ==================================================================== */
/*                            TILRasterBand                             */
/* ==================================================================== */
/************************************************************************/

class TILRasterBand : public GDALPamRasterBand
{
    friend class TILDataset;

    VRTRasterBand *poVRTBand;

  public:
                   TILRasterBand( TILDataset *, int, VRTRasterBand * );
    virtual       ~TILRasterBand() {};

    virtual CPLErr IReadBlock( int, int, void * );
};

/************************************************************************/
/*                           TILRasterBand()                            */
/************************************************************************/

TILRasterBand::TILRasterBand( TILDataset *poTILDS, int nBand, 
                              VRTRasterBand *poVRTBand )

{
    this->poDS = poTILDS;
    this->poVRTBand = poVRTBand;
    this->nBand = nBand;
    this->eDataType = poVRTBand->GetRasterDataType();

    poVRTBand->GetBlockSize( &nBlockXSize, &nBlockYSize );
}

/************************************************************************/
/*                             IReadBlock()                             */
/************************************************************************/

CPLErr TILRasterBand::IReadBlock( int iBlockX, int iBlockY, void *pBuffer )

{
    return poVRTBand->ReadBlock( iBlockX, iBlockY, pBuffer );
}

/************************************************************************/
/* ==================================================================== */
/*                             TILDataset                               */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                             TILDataset()                             */
/************************************************************************/

TILDataset::TILDataset()

{
    poVRTDS = NULL;
}

/************************************************************************/
/*                            ~TILDataset()                             */
/************************************************************************/

TILDataset::~TILDataset()

{
    if( poVRTDS )
        delete poVRTDS;

    while( !apoTileDS.empty() )
    {
        GDALClose( (GDALDatasetH) apoTileDS.back() );
        apoTileDS.pop_back();
    }
}

/************************************************************************/
/*                              Identify()                              */
/************************************************************************/

int TILDataset::Identify( GDALOpenInfo *poOpenInfo )

{
    if( poOpenInfo->nHeaderBytes < 200 
        || !EQUAL(CPLGetExtension(poOpenInfo->pszFilename),"TIL") )
        return FALSE;

    if( strstr((const char *) poOpenInfo->pabyHeader,"numTiles") == NULL )
        return FALSE;
    else
        return TRUE;
}

/************************************************************************/
/*                                Open()                                */
/************************************************************************/

GDALDataset *TILDataset::Open( GDALOpenInfo * poOpenInfo )

{
    if( !Identify( poOpenInfo ) )
        return NULL;

/* -------------------------------------------------------------------- */
/*      Confirm the requested access is supported.                      */
/* -------------------------------------------------------------------- */
    if( poOpenInfo->eAccess == GA_Update )
    {
        CPLError( CE_Failure, CPLE_NotSupported, 
                  "The TIL driver does not support update access to existing"
                  " datasets.\n" );
        return NULL;
    }
    
    CPLString osDirname = CPLGetDirname(poOpenInfo->pszFilename);

/* -------------------------------------------------------------------- */
/*      Try to find the corresponding .IMD file.                        */
/* -------------------------------------------------------------------- */
    char **papszIMD = GDALLoadIMDFile( poOpenInfo->pszFilename, 
                                       poOpenInfo->papszSiblingFiles );

    if( papszIMD == NULL )
    {
        CPLError( CE_Failure, CPLE_OpenFailed,
                  "Unable to open .TIL dataset due to missing .IMD file." );
        return NULL;
    }

    if( CSLFetchNameValue( papszIMD, "numRows" ) == NULL
        || CSLFetchNameValue( papszIMD, "numColumns" ) == NULL
        || CSLFetchNameValue( papszIMD, "bitsPerPixel" ) == NULL )
    {
        CPLError( CE_Failure, CPLE_OpenFailed,
                  "Missing a required field in the .IMD file." );
        CSLDestroy( papszIMD );
        return NULL;
    }

/* -------------------------------------------------------------------- */
/*      Try to load and parse the .TIL file.                            */
/* -------------------------------------------------------------------- */
    FILE *fp = VSIFOpenL( poOpenInfo->pszFilename, "r" );
    
    if( fp == NULL )
    {
        CSLDestroy( papszIMD );
        return NULL;
    }

    CPLKeywordParser oParser;

    if( !oParser.Ingest( fp ) )
    {
        VSIFCloseL( fp );
        CSLDestroy( papszIMD );
        return NULL;
    }

    VSIFCloseL( fp );

    char **papszTIL = oParser.GetAllKeywords();

/* -------------------------------------------------------------------- */
/*      Create a corresponding GDALDataset.                             */
/* -------------------------------------------------------------------- */
    TILDataset 	*poDS;

    poDS = new TILDataset();

    poDS->nRasterXSize = atoi(CSLFetchNameValueDef(papszIMD,"numColumns","0"));
    poDS->nRasterYSize = atoi(CSLFetchNameValueDef(papszIMD,"numRows","0"));
    if (!GDALCheckDatasetDimensions(poDS->nRasterXSize, poDS->nRasterYSize))
    {
        delete poDS;
        CSLDestroy( papszIMD );
        return NULL;
    }

/* -------------------------------------------------------------------- */
/*      We need to open one of the images in order to establish         */
/*      details like the band count and types.                          */
/* -------------------------------------------------------------------- */
    GDALDataset *poTemplateDS;
    const char *pszFilename = CSLFetchNameValue( papszTIL, "TILE_1.filename" );
    if( pszFilename == NULL )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "Missing TILE_1.filename in .TIL file." );
        delete poDS;
        CSLDestroy( papszIMD );
        return NULL;
    }

    // trim double quotes. 
    if( pszFilename[0] == '"' )
        pszFilename++;
    if( pszFilename[strlen(pszFilename)-1] == '"' )
        ((char *) pszFilename)[strlen(pszFilename)-1] = '\0';

    CPLString osFilename = CPLFormFilename(osDirname, pszFilename, NULL);
    poTemplateDS = (GDALDataset *) GDALOpen( osFilename, GA_ReadOnly );
    if( poTemplateDS == NULL || poTemplateDS->GetRasterCount() == 0)
    {
        delete poDS;
        CSLDestroy( papszIMD );
        if (poTemplateDS != NULL)
            GDALClose( poTemplateDS );
        return NULL;
    }

    GDALRasterBand *poTemplateBand = poTemplateDS->GetRasterBand(1);
    GDALDataType eDT = poTemplateBand->GetRasterDataType();
    int          nBandCount = poTemplateDS->GetRasterCount();

    poTemplateBand = NULL;
    GDALClose( poTemplateDS );

/* -------------------------------------------------------------------- */
/*      Create and initialize the corresponding VRT dataset used to     */
/*      manage the tiled data access.                                   */
/* -------------------------------------------------------------------- */
    int iBand;
     
    poDS->poVRTDS = new VRTDataset(poDS->nRasterXSize,poDS->nRasterYSize);

    for( iBand = 0; iBand < nBandCount; iBand++ )
        poDS->poVRTDS->AddBand( eDT, NULL );

    /* Don't try to write a VRT file */
    poDS->poVRTDS->SetWritable(FALSE);

/* -------------------------------------------------------------------- */
/*      Create band information objects.                                */
/* -------------------------------------------------------------------- */
    for( iBand = 1; iBand <= nBandCount; iBand++ )
        poDS->SetBand( iBand, 
                       new TILRasterBand( poDS, iBand, 
                (VRTRasterBand *) poDS->poVRTDS->GetRasterBand(iBand)));

/* -------------------------------------------------------------------- */
/*      Add tiles as sources for each band.                             */
/* -------------------------------------------------------------------- */
    int nTileCount = atoi(CSLFetchNameValueDef(papszTIL,"numTiles","0"));
    int iTile = 0;

    for( iTile = 1; iTile <= nTileCount; iTile++ )
    {
        CPLString osKey;

        osKey.Printf( "TILE_%d.filename", iTile );
        pszFilename = CSLFetchNameValue( papszTIL, osKey );
        if( pszFilename == NULL )
        {
            CPLError( CE_Failure, CPLE_AppDefined,
                      "Missing TILE_%d.filename in .TIL file.", iTile );
            delete poDS;
            CSLDestroy( papszIMD );
            return NULL;
        }
        
        // trim double quotes. 
        if( pszFilename[0] == '"' )
            pszFilename++;
        if( pszFilename[strlen(pszFilename)-1] == '"' )
            ((char *) pszFilename)[strlen(pszFilename)-1] = '\0';
        osFilename = CPLFormFilename(osDirname, pszFilename, NULL);

        osKey.Printf( "TILE_%d.ULColOffset", iTile );
        int nULX = atoi(CSLFetchNameValueDef(papszTIL, osKey, "0"));
        
        osKey.Printf( "TILE_%d.ULRowOffset", iTile );
        int nULY = atoi(CSLFetchNameValueDef(papszTIL, osKey, "0"));

        osKey.Printf( "TILE_%d.LRColOffset", iTile );
        int nLRX = atoi(CSLFetchNameValueDef(papszTIL, osKey, "0"));
        
        osKey.Printf( "TILE_%d.LRRowOffset", iTile );
        int nLRY = atoi(CSLFetchNameValueDef(papszTIL, osKey, "0"));

#ifdef notdef
        GDALDataset *poTileDS = (GDALDataset *) 
            GDALOpen(osFilename,GA_ReadOnly);
#else
        GDALDataset *poTileDS = 
            new GDALProxyPoolDataset( osFilename, 
                                      nLRX - nULX + 1, nLRY - nULY + 1 );
#endif
        if( poTileDS == NULL )
            continue;

        poDS->apoTileDS.push_back( poTileDS );

        for( iBand = 1; iBand <= nBandCount; iBand++ )
        {
#ifndef notdef
            ((GDALProxyPoolDataset *) poTileDS)->
                AddSrcBandDescription( eDT, nLRX - nULX + 1, 1 );
#endif            
            GDALRasterBand *poSrcBand = poTileDS->GetRasterBand(iBand);

            VRTSourcedRasterBand *poVRTBand = 
                (VRTSourcedRasterBand *) poDS->poVRTDS->GetRasterBand(iBand);
            
            poVRTBand->AddSimpleSource( poSrcBand,
                                        0, 0, 
                                        nLRX - nULX + 1, nLRY - nULY + 1, 
                                        nULX, nULY, 
                                        nLRX - nULX + 1, nLRY - nULY + 1 );
        }
    }

/* -------------------------------------------------------------------- */
/*      Set RPC and IMD metadata.                                       */
/* -------------------------------------------------------------------- */
    char **papszRPCMD = GDALLoadRPBFile( poOpenInfo->pszFilename,
                                         poOpenInfo->papszSiblingFiles );
        
    if( papszRPCMD != NULL )
    {
        poDS->SetMetadata( papszRPCMD, "RPC" );
        CSLDestroy( papszRPCMD );
    }

    if( papszIMD != NULL )
        poDS->SetMetadata( papszIMD, "IMD" );

/* -------------------------------------------------------------------- */
/*      Cleanup                                                         */
/* -------------------------------------------------------------------- */
    CSLDestroy( papszIMD );

/* -------------------------------------------------------------------- */
/*      Initialize any PAM information.                                 */
/* -------------------------------------------------------------------- */
    poDS->SetDescription( poOpenInfo->pszFilename );
    poDS->TryLoadXML();

/* -------------------------------------------------------------------- */
/*      Check for overviews.                                            */
/* -------------------------------------------------------------------- */
    poDS->oOvManager.Initialize( poDS, poOpenInfo->pszFilename );

    return( poDS );
}

/************************************************************************/
/*                          GDALRegister_TIL()                          */
/************************************************************************/

void GDALRegister_TIL()

{
    GDALDriver	*poDriver;

    if( GDALGetDriverByName( "TIL" ) == NULL )
    {
        poDriver = new GDALDriver();
        
        poDriver->SetDescription( "TIL" );
        poDriver->SetMetadataItem( GDAL_DMD_LONGNAME, 
                                   "EarthWatch .TIL" );
        poDriver->SetMetadataItem( GDAL_DMD_HELPTOPIC, 
                                   "frmt_til.html" );
        
        poDriver->pfnOpen = TILDataset::Open;
        poDriver->pfnIdentify = TILDataset::Identify;

        GetGDALDriverManager()->RegisterDriver( poDriver );
    }
}


