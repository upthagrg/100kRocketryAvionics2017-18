/*******************************************************************************************
*Title: conversion.h
*Author: Glenn Upthagrove
*Date: 03/07/2018
*Description: Converts raw telemetry data into JSON formatted strings. 
*******************************************************************************************/

#ifndef __CONVERSION
#define __CONVERSION


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "./telemetry.h"
int debug;
//void convert(char* buff, float vel, float lat, float lon, float alt, float time){

/***********************************************
*Title: convert
*Params: a string pointer, a telem_data*
*Description: Converts a telem_data struct into 
*a JSON packet
***********************************************/
void convert(char** buff, struct telem_data* in);

/***********************************************
*Title: structure
*Params: a string pointer
*Description: Converts a JSON packet into a 
*telem_data structure
***********************************************/
struct telem_data structure(char** buff);

#endif
