/*******************************************************************************************
*Title: conversion.h
*Author: Glenn Upthagrove
*Date: 01/05/2018
*Description: Converts raw telemetry data into JSON formatted strings. 
*******************************************************************************************/
#define _XOPEN_SOURCE 500 //makes usleep work
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
int debug;
void convert(char* buff, float vel, float lat, float lon, float alt, float time){
	char str1[9] = "velocity";
	char str2[9] = "latitude";
	char str3[10] = "longitude";
	char str4[9] = "altitude";
	char str5[5] = "time";
	if(buff == NULL){
		buff = (char*)malloc(256);
	}
	memset(buff, '\0', 256);
	sprintf(buff,"'{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"+%f\"}'", str1,vel,str2,lat,str3,lon,str4,alt,str5,time); //make JSON string
	if(debug){
		printf("%s\n", buff);
	}
}
