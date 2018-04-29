/*******************************************************************************************
*Title: conversion.c
*Author: Glenn Upthagrove
*Date: 01/05/2018
*Description: Conversion tests
*******************************************************************************************/
#define _XOPEN_SOURCE 500 //makes usleep work
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "./conversion.h"

void gen_data(float freq, float salt, float svel, float slat, float slon){
	char str1[9] = "velocity";
	char str2[9] = "latitude";
	char str3[10] = "longitude";
	char str4[9] = "altitude";
	char str5[5] = "time";
	char str6[5] = "type";
	char buff[256];
	char* buff2 = NULL;
	struct telem_data data;
	struct telem_data data2;
	data.vel=svel; //initial velocity, no acceleration other than gravity, no jerk
	data.lat=slat;
	data.lon=slon;
	data.alt=salt;
	data.type='b';
	data.time=0.0;
	int dec;
	buff2 = malloc(sizeof(char)*256);
	do{
		memset(buff, '\0', 256);
		memset(buff2, '\0', 256);
		sprintf(buff,"%f %f %f %f %f %c", data.vel,data.lat,data.lon,data.alt,data.time, data.type); //make ASCII string
		if(debug){
			printf("%s\n", buff); //print JSON string
		}
		convert(&buff2, &data);
		if(debug){
			printf("%s\n", buff2); //print JSON string
		}
		data2 = structure(&buff2);
		if(debug){
			printf("struct:\n");
			printf("lat: %f\n", data2.lat);
			printf("lon: %f\n", data2.lon);
			printf("vel: %f\n", data2.vel);
			printf("alt: %f\n", data2.alt);
			printf("time: %f\n", data2.time);
			printf("type: %c\n", data2.type);
		}
		data.time += (1.0/freq); //update time
		data.alt = ((-4.9*powf(data.time, 2)) + (svel*data.time) + salt); //update alt
		dec = rand()%2;
		if(dec){ //update lat
			data.lat += 0.001;
		}
		else{
			data.lat -= 0.001;
		}
		dec = rand()%2;
		if(dec){ //update lon
			data.lon += 0.001;
		}
		else{
			data.lon -= 0.001;
		}
		data.vel = ((-9.8*data.time) + (svel)); //update vel
		usleep(1000000/freq);
	}while(data.alt>salt);
	//printf("**\n");
	if(buff2 !=NULL){
		free(buff2);
		buff2 = NULL;
	}
}

int main(int argc, char** argv){
	float freq = 1;
	float salt = 0.0;
	float svel = 100.0;
	float slat = 45.0;
	float slon = 45.0;
	int i=0;
	debug = 0;
	srand(time(NULL));
	for(i; i<argc; i++){
		if(strcmp(argv[i], "-vel") == 0){ //get starting velocity
			svel = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-alt") == 0){ //get starting altitude
			salt = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-lat") == 0){ //get starting latitude
			slat = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-lon") == 0){ //get starting longitude
			slon = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-rate") == 0){ //get update rate in Hz
			freq = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-debug") == 0){ //get update rate in Hz
			debug = 1;
			printf("conversion debug: on\n");
		}
	}
	gen_data(freq, salt, svel, slat, slon);
	return 0;
}
