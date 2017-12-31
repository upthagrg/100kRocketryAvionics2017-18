/*******************************************************************************************
*Title: logger.c
*Author: Glenn Upthagrove
*Date: 12/28/2017
*Description: A grogram that logs incoming telemetry data to a file. 
*******************************************************************************************/
#define _XOPEN_SOURCE 500 //makes usleep work
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
float freq;
float alt;
float vel; 
float lat;
float lon;
float Time1;

void gen_data2(char* buff, float svel, float salt, float slat, float slon){
	char str1[9] = "velocity";
	char str2[9] = "latitude";
	char str3[10] = "longitude";
	char str4[9] = "altitude";
	char str5[5] = "time";
//	char* buff;
	int dec;
	//	buff = (char*)malloc(sizeof(char)*256);
		memset(buff, '\0', 256);
		sprintf(buff,"'{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"+%f\"}'", str1,vel,str2,lat,str3,lon,str4,alt,str5,Time1); //make JSON string
		//printf("%s\n", buff); //print JSON string
	//	printf("time: %f\n", Time1);
		Time1 += (1.0/freq); //update time
	//	printf("time: %f\n", Time1);
	//	printf("alt: %f\n", alt);
		alt = ((-4.9*powf(Time1, 2)) + (svel*Time1) + salt); //update alt
	//	printf("alt: %f\n", alt);
		dec = rand()%2;
		if(dec){ //update lat
			lat += 0.001;
		}
		else{
			lat -= 0.001;
		}
		dec = rand()%2;
		if(dec){ //update lon
			lon += 0.001;
		}
		else{
			lon -= 0.001;
		}
		vel = ((-9.8*Time1) + (svel)); //update vel
		//usleep(1000000/freq);
	//	return buff; //return JSON string
	
}

int main(int argc, char** argv){
	//freq = 1;
	int i=0;
	char* str;
	float salt = 0.0;
	float svel = 100.0;
	float slat = 45.0;
	float slon = 45.0;
	Time1 = 0.0;
	vel = svel;
	lat = slat;
	lon = slon;
	alt = salt;
	freq = 1;
	
	srand(time(NULL));
	str = (char*)malloc(sizeof(char)*256);

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
	}
	do{
		gen_data2(str, svel, salt, slon, slat);
		printf("%s\n", str);
		usleep(1000000/freq);
	}while(alt>=salt);
	return 0;
}
