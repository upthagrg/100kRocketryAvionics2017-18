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
void convert(char* buff, struct telem_data* in){
	char str1[9] = "velocity";
	char str2[9] = "latitude";
	char str3[10] = "longitude";
	char str4[9] = "altitude";
	char str5[5] = "time";
	if(buff == NULL){
		buff = (char*)malloc(256);
	}
	memset(buff, '\0', 256);
	sprintf(buff,"'{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"+%f\"}'", str1,in->vel,str2,in->lat,str3,in->lon,str4,in->alt,str5,in->time); //make JSON string
	if(debug){
		printf("%s\n", buff);
	}
}

struct telem_data structure(char* buff){
	char* messagecpy;
	char* token;
	struct telem_data data;

	printf("in structure\n");

printf("alloc token\n");
	token = malloc(sizeof(char)*256);
printf("alloc cpy\n");
	messagecpy = malloc(sizeof(char)*256);
printf("memset token\n");
	memset(token, '\0', 256);
printf("memset cpy\n");
	memset(messagecpy, '\0', 256);

printf("cpy\n");
	strcpy(messagecpy, buff);
printf("first token\n");
	token = strtok(messagecpy, ":");
	printf("%s\n", token);
//printf("memset token\n");
//	memset(token, '\0', 256);
        token = strtok(NULL, ":");
	printf("%s\n", token);
//printf("memset token\n");
//	memset(token, '\0', 256);
        token = strtok(NULL, "\"");
	printf("%s\n", token);

        //temprd.z = atof(token);
//printf("memset token\n");
//	memset(token, '\0', 256);
        token = strtok(NULL, ":");
	printf("%s\n", token);
//printf("memset token\n");
//	memset(token, '\0', 256);
        token = strtok(NULL, "\"");
	printf("%s\n", token);
        //temprd.x = atof(token);
//printf("memset token\n");
//	memset(token, '\0', 256);
        token = strtok(NULL, ":");
	printf("%s\n", token);
//printf("memset token\n");
//	memset(token, '\0', 256);
        token = strtok(NULL, "\"");
	printf("%s\n", token);
        //temprd.y = atof(token);
        //temprd.x = (-(slon - temprd.x) * (cos(slat*(M_PI/180.0))*69.172));
        //temprd.y = temprd.y / 5280.0;
        //temprd.z = ((slat - temprd.z)*69.0);
        //temprd.x *= 10;
        //temprd.y *= 10;
        //temprd.z *= 10;
	printf("leaving structure\n");
	return data;
}
