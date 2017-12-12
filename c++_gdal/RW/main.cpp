#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void writeTileData(int StartRow, int StartColumn, int NbrRows, int NbrColumns,
                   float * TileData,
                   double adfGeoTransform[6],
                   GDALDataset * srcDataset){

    const char * pszFormat = "GTiff";
    GDALDriver * poDriver;
    //char **papszMetadata;
    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
    if( poDriver == NULL )
        exit( 1 );
    //papszMetadata = poDriver->GetMetadata();

    GDALDataset * poDstDataset;
    char **papszOptions = NULL;

    std::string FileName ("/home/peter/Desktop/TestData/test");
    std::string FileSuffix (".tif");
    std::stringstream s1, s2;
    std::string StartRowStr, StartColumnStr;

    if (StartRow == 0){
        StartRowStr = "000";
    }else{
        s1 << StartRow;
        StartRowStr = s1.str();
    }
    if (StartColumn == 0){
        StartColumnStr = "000";
    }else{
        s2 << StartColumn;
        StartColumnStr = s2.str();
    }

    FileName += StartRowStr + StartColumnStr + FileSuffix;

    poDstDataset = poDriver->Create( FileName.c_str(), NbrRows, NbrColumns, 1, GDT_Byte,
                                papszOptions );

    GDALRasterBand *poDstBand;
    poDstDataset->SetGeoTransform( adfGeoTransform );
    poDstDataset->SetProjection(srcDataset->GetProjectionRef());

    poDstBand = poDstDataset->GetRasterBand(1);

    poDstBand->RasterIO( GF_Write, 0, 0, NbrRows, NbrColumns,
                      TileData, NbrRows, NbrColumns, GDT_Float32, 0, 0 );

    GDALClose( (GDALDatasetH) poDstDataset );

}

void readTileData(int StartRow, int StartColumn, int EndRow, int EndColumn, GDALDataset * poDataset){

    float * TileData;
    GDALRasterBand *poBand;
    poBand = poDataset->GetRasterBand( 1 );

    int nXSize = EndRow - StartRow;
    int nYSize = EndColumn - StartColumn;

    TileData = (float *) CPLMalloc(sizeof(float)*nXSize*nYSize);

    poBand->RasterIO( GF_Read, StartRow, StartColumn, nXSize, nYSize,
                      TileData, nXSize, nYSize, GDT_Float32,
                      0, 0 );

    double adfGeoTransform[6];
    poDataset->GetGeoTransform( adfGeoTransform );
    adfGeoTransform[0] += StartRow * adfGeoTransform[1];
    adfGeoTransform[3] += StartColumn * adfGeoTransform[5];

    writeTileData(StartRow, StartColumn, nXSize, nYSize, TileData, adfGeoTransform, poDataset);

}



void tiledata(GDALDataset * srcDataset){

    // iterate over Image
    int TileSize = 200;
    int StartRow, EndRow, StartColumn, EndColumn = 0;

    for(int i = 0; i < srcDataset->GetRasterXSize(); i += TileSize){
        for(int j = 0; j < srcDataset->GetRasterYSize(); j += TileSize){

            StartRow = i;
            StartColumn = j;

            if(srcDataset->GetRasterYSize() < StartColumn + TileSize){
                EndColumn = srcDataset->GetRasterYSize();
            }else{
                EndColumn = StartColumn + TileSize;
            }

            if(srcDataset->GetRasterXSize() < StartRow + TileSize){
                EndRow = srcDataset->GetRasterXSize();
            }else{
                EndRow = StartRow + TileSize;
            }
            readTileData(StartRow, StartColumn, EndRow, EndColumn, srcDataset);
        }
    }

    GDALClose( (GDALDatasetH) srcDataset );
}

int main(int argc, char *argv[])
{

    GDALDataset  * srcDataset;
    GDALAllRegister();
    srcDataset = (GDALDataset *) GDALOpen( "/home/peter/Desktop/SP27GTIF.TIF", GA_ReadOnly );

    tiledata(srcDataset);

    return 0;
}
