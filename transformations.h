#ifndef TRANSFORMATIONS_H_   
#define TRANSFORMATIONS_H_

#include "a2methods.h"
#include "pnm.h"

extern void applyTransform(Pnm_ppm sourceImg, int rotationType, char* mapType, 
                            A2Methods_T methods, char* time_file_name);

#endif