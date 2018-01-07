/*******************************************************************************************
*Title: logtest1.c
*Author: Glenn Upthagrove
*Date: 01/01/2018
*Description: A grogram that test the logging function. This one just tests basic
*functionality visually. 
*******************************************************************************************/
#define _XOPEN_SOURCE 500 //makes usleep work
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "./logger.h"

int main(int argc, char** argv){
        int i=0;
        FILE* logfile;
        salt = 0.0;
        svel = 100.0;
        slat = 45.0;
        slon = 45.0;
        fname = NULL;
        debug = 0;
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
                else if(strcmp(argv[i], "-out") == 0){ //get output file name
                        fname = argv[i+1];
                }
                else if(strcmp(argv[i], "-debug") == 0){ //get output file name
                        debug = 1;
                }
                if(fname == NULL){
                        fname = "./log.txt";
                }
        }
        //place unit test here

        logfile = fopen(fname, "w+");
        logfun2(logfile);
	


        fclose(logfile);

        return 0;
}

