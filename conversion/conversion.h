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
#include "./telemetry.h"
int debug;
//void convert(char* buff, float vel, float lat, float lon, float alt, float time){
<<<<<<< HEAD
void convert(char* buff, struct telem_data* in){
=======
void convert(char** buff, struct telem_data* in){
>>>>>>> 5917ef788261dc3584a2093789f54d6ae1e51656
	char str1[9] = "velocity";
	char str2[9] = "latitude";
	char str3[10] = "longitude";
	char str4[9] = "altitude";
	char str5[5] = "time";
	if(buff == NULL){
<<<<<<< HEAD
		buff = (char*)malloc(256);
	}
	memset(buff, '\0', 256);
	sprintf(buff,"'{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"+%f\"}'", str1,in->vel,str2,in->lat,str3,in->lon,str4,in->alt,str5,in->time); //make JSON string
	if(debug){
		printf("%s\n", buff);
	}
}
=======
		*buff = (char*)malloc(256);
	}
	memset(*buff, '\0', 256);
	sprintf(*buff,"'{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"+%f\"}'", str1,in->vel,str2,in->lat,str3,in->lon,str4,in->alt,str5,in->time); //make JSON string
	if(debug){
		printf("%s\n", *buff);
	}
}

struct telem_data structure(char** buff){
	char* messagecpy;
	char* token;
	struct telem_data data;


	token = malloc(sizeof(char)*256);
	messagecpy = malloc(sizeof(char)*256);
	memset(token, '\0', 256);
	memset(messagecpy, '\0', 256);
	strcpy(messagecpy, *buff);
	token = strtok(messagecpy, ":");
	token = strtok(NULL, "\"");
	data.vel = atof(token);
        token = strtok(NULL, ":");
        token = strtok(NULL, "\"");
	data.lat = atof(token);
        token = strtok(NULL, ":");
        token = strtok(NULL, "\"");
	data.lon = atof(token);
        token = strtok(NULL, ":");
        token = strtok(NULL, "\"");
	data.alt = atof(token);
        token = strtok(NULL, ":");
        token = strtok(NULL, "\"");
	data.time = atof(token);
	return data;
}
>>>>>>> 5917ef788261dc3584a2093789f54d6ae1e51656
