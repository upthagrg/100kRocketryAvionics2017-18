/*******************************************************************************************
*Title: logger.h
*Author: Glenn Upthagrove
*Date: 01/01/2018
*Description: Header for a grogram that logs incoming telemetry data to a file. 
*******************************************************************************************/
#ifndef LOGGER
#define LOGGER

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
char* str;
float salt;
float svel;
float slat;
float slon;
char* fname;
int debug;
int buffsize;

void gen_data(char* buff, float svel, float salt, float slat, float slon){
        char str1[9] = "velocity";
        char str2[9] = "latitude";
        char str3[10] = "longitude";
        char str4[9] = "altitude";
        char str5[5] = "time";
//      char* buff;
        int dec;
        //      buff = (char*)malloc(sizeof(char)*256);
                memset(buff, '\0', 256);
                sprintf(buff,"'{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"+%f\"}'", str1,vel,str2,lat,str3,lon,str4,alt,str5,Time1); //make JSON string
                //printf("%s\n", buff); //print JSON string
        //      printf("time: %f\n", Time1);
                Time1 += (1.0/freq); //update time
        //      printf("time: %f\n", Time1);
        //      printf("alt: %f\n", alt);
                alt = ((-4.9*powf(Time1, 2)) + (svel*Time1) + salt); //update alt
        //      printf("alt: %f\n", alt);
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
        //      return buff; //return JSON string

}
//real version reading from stdin (usually a pipe)
void logfun(FILE* logfile){
	char* buff;
//	char buff[64];
	char message[256];
	int bytes;
	int cnt = 0;
	char* end = NULL;
	buff = (char*)malloc(buffsize);
	if(debug){
		printf("in logfun\n");
		fflush(stdout);
		printf("buffsize: %d\n", buffsize);
		fflush(stdout);
	}
	do{
		memset(message, '\0', sizeof(message));
		if(debug){
			cnt++;
		}
	//	if(cnt >= 100){
	//		printf("failsafe breaking\n");
	//		fflush(stdout);
	//		break;
	//	}
		//gen_data(str, svel, salt, slon, slat);
		while(strstr(message, "&&") == NULL){
			//memset(buff, '\0', sizeof(buff));
			memset(buff, '\0', buffsize);
			if(debug){
				printf("logger is reading\n`");
			//	printf("sizeof(buff): %d\n",sizeof(buff));
				fflush(stdout);
			}
			//bytes = read(3, buff, sizeof(buff)-1); //reads from file 3 (should be a pipe)
			bytes = read(3, buff, buffsize-1); //reads from file 3 (should be a pipe)
			strcat(message, buff); // append the buffer to the message
			if(bytes == -1){ //error cases
				printf("READ ERROR IN LOGGER, RETURN OF -1\n");
				fflush(stdout);
				exit(4);
			}
			if(bytes == 0){
				printf("NO READ IN LOGGER, RETURN OF 0\n");
				fflush(stdout);
				exit(5);
			}
			if(debug){
				printf("logger Buffer: %s logger Message: %s\n`", buff, message);
				fflush(stdout);
			}
		}
		//fix the string
		end = strstr(message, "&&"); //points to first &
		*end = '\0'; //null terminate
		
		if((strcmp(message, "**") == 0) || (strcmp(message, "**&&") == 0)){ //exit if all data transmitted 
			if(debug){
				printf("logger recieved end of data transmit signal and is exiting\n");
				fflush(stdout);
			}
				printf("logger recieved end of data transmit signal and is exiting\n");
			break;
		}
		if(debug){
			printf("logger is writing: %s to file\n", message);
			fflush(stdout);
		}
		fprintf(logfile, message); //write full message to file
		fprintf(logfile, "\n");
		if(debug){
			printf("logger finished writing to file\n");
			fflush(stdout);
		}

		//no need to sleep, hangs on reading from stdin (usually a pipe)	
		//usleep((1000000/freq)+1);

	}while(1);

}
//test version using gen_data function
void logfun2(FILE* logfile){
	do{
		gen_data(str, svel, salt, slon, slat);
		if(debug){
			printf("%s\n`", str);
		}
		fprintf(logfile, str);	
		fprintf(logfile, "\n");	
		usleep(1000000/freq);

	}while(alt>=salt);

}
#endif
