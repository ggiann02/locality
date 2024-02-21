#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "assert.h"
#include "transformations.h"


static A2Methods_UArray2 createNewImg(Pnm_ppm sourceImg, int rotationType);
static void rotate90(int col, int row, A2Methods_UArray2 array, void *elem, void *cl);
static void rotate180(int col, int row, A2Methods_UArray2 array, void *elem, void *cl);
static void rotate270(int col, int row, A2Methods_UArray2 array, void *elem, void *cl);


void applyTransform(Pnm_ppm sourceImg, int rotationType, char* mapType, 
                    A2Methods_T methods, char* time_file_name)
{
    CPUTime_T timer;
    double time_used; 
    timer = CPUTime_New();

    (void) time_file_name;
    // start timer here

    // check if rotation type is 0, if it's not we create the new empty image
    (void) mapType;
    A2Methods_UArray2 finalImg = createNewImg(sourceImg, rotationType);
    CPUTime_Start(timer);

    if (rotationType == 90) {
        sourceImg->methods->map_default(finalImg, rotate90, sourceImg);
    } else if (rotationType == 180) {
        sourceImg->methods->map_default(finalImg, rotate180, sourceImg);
    } else if (rotationType == 270) {
        sourceImg->methods->map_default(finalImg, rotate270, sourceImg);
    }
    time_used = CPUTime_Stop(timer);

    sourceImg->methods->free(&(sourceImg->pixels));
    sourceImg->pixels = finalImg;
    sourceImg->width = methods->width(finalImg);
    sourceImg->height = methods->height(finalImg);

    Pnm_ppmwrite(stdout, sourceImg);

    (void) methods;

    CPUTime_Free(&timer);     
}        

A2Methods_UArray2 createNewImg(Pnm_ppm sourceImg, int rotationType) 
{
    int width; int height; int size;

    if (rotationType == 180) {
        width = sourceImg->methods->width(sourceImg->pixels);
        height = sourceImg->methods->height(sourceImg->pixels);
        size = sourceImg->methods->size(sourceImg->pixels);

    } else if (rotationType == 90 || rotationType == 270) {
        width = sourceImg->methods->height(sourceImg->pixels);
        height = sourceImg->methods->width(sourceImg->pixels);
        size = sourceImg->methods->size(sourceImg->pixels);
    }

    return sourceImg->methods->new(width, height, size);

}


// FUNCTION TO ROTATE 90

void rotate90(int col, int row, A2Methods_UArray2 array, void *elem, void *cl) 
{
    (void) elem;
    (void) array;

    // closure is the new image file

    Pnm_ppm finalImg = cl;

    int newCol = finalImg->height - col - 1;
    int newRow = row;

    *(Pnm_rgb) elem = *(Pnm_rgb) finalImg->methods->at(finalImg->pixels, newRow, newCol);
}

void rotate180(int col, int row, A2Methods_UArray2 array, void *elem, void *cl) 
{
    (void) array;
    (void) elem;

    // closure is the new image file

    Pnm_ppm finalImg = cl;

    int newCol = finalImg->width - col - 1;
    int newRow = finalImg->height - row - 1;

    *(Pnm_rgb) elem = *(Pnm_rgb) finalImg->methods->at(finalImg->pixels, newCol, newRow);
}

void rotate270(int col, int row, A2Methods_UArray2 array, void *elem, void *cl) 
{
    (void) elem;
    (void) array;

    // closure is the new image file

    Pnm_ppm finalImg = cl;

    int newCol = finalImg->width - row - 1;
    int newRow = col;

    *(Pnm_rgb) elem = *(Pnm_rgb) finalImg->methods->at(finalImg->pixels, newCol, newRow);
}

void makeTimingFile(char* time_file_name, double time_used, )