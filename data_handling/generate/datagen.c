/*******************************************************************************************
*Title: datagen.c
*Author: Glenn Upthagrove
*Date: 12/20/2017
*Description: A semi-random generator of JSON formatted data for testing other pieces of the 
*tracking software. The altitude is a simple quadratic, and the velocity is the derivative
*thereof. The rate of update can be set via -rate, the initial altitude via -alt, the 
*initial latitude via -lat, the initial longitude via -lon, and the initial velocity via 
*-vel. If any of thee are not set, defaults are used.
*******************************************************************************************/
#define _XOPEN_SOURCE 500 //makes usleep work
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


//int files[2];
int fifo;
char* fifoname = "./commfifo";
int debug;
char* name;
void gen_data(float freq, float salt, float svel, float slat, float slon){
	char str1[9] = "velocity";
	char str2[9] = "latitude";
	char str3[10] = "longitude";
	char str4[9] = "altitude";
	char str5[5] = "time";
	char buff[256];
	char buff3[256];
	char* buff2;
	float vel=svel; //initial velocity, no acceleration other than gravity, no jerk
	float lat=slat;
	float lon=slon;
	float alt=salt;
	float time=0.0;
	int dec;
	int wret=-5;
	int total=0;
	//printf("file: %d\n", file);
	if(debug){
		printf("starting data generation\n");
		fflush(stdout);
	}
	do{ //loop until flight is "finished" 
		total = 0;
		memset(buff, '\0', 256);
		sprintf(buff,"{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"+%f\"}", str1,vel,str2,lat,str3,lon,str4,alt,str5,time); //make JSON string
//		sprintf(buff3,"'{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"+%f\"}'", str1,vel,str2,lat,str3,lon,str4,alt,str5,time); //make JSON string
		//fprintf(files[1], buff); //print JSON string
		//fprintf(files[1], "\n"); //print newline
		buff2 = buff;
		while(total < strlen(buff2)){ //loop until total message is written
			fifo = open(fifoname, O_WRONLY);
			if(debug){
				printf("data_gen writing\n");
				fflush(stdout);
				printf("%s\n", buff2);
				fflush(stdout);
			}
			wret = write(fifo, buff2, (strlen(buff2))); //write to FIFO
			//if(file != -5){
			//	printf("%s\n", buff3);
			//}
			if(debug){
				printf("data_gen finished writing\n");
				fflush(stdout);
			}
			if(wret == -1){
				printf("WRITE ERROR TO FIFO IN GEN_DATA\n");
				fflush(stdout);
				exit(1);
			}
			total += wret; //track total written
			//write(files[1], "\n", 1);
			if(debug){
				printf("data_gen Finished, size of write: %d, should be %lu\n", wret, strlen(buff2));
				fflush(stdout);
				printf("data_gen Finished, size of write: %d, total writen %d\n", wret, total);
				fflush(stdout);
			}
			buff2 += wret; //move up the string
		}
		close(fifo);
		//update variables
		time += (1.0/freq); //update time
		alt = ((-4.9*powf(time, 2)) + (svel*time) + salt); //update alt
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
		vel = ((-9.8*time) + (svel)); //update vel
		usleep(1000000/freq);

		if(debug){
			printf("data_gen alt: %f salt: %f\n", alt, salt);
			fflush(stdout);
		}
	}while(alt>salt);
	if(debug){
		printf("data_gen finished generating data, sending end of transmit signal\n");
		fflush(stdout);
	}
	fifo = open(fifoname, O_WRONLY);
	wret = write(fifo, "**&&", 4);
	close(fifo);
	if(debug){
		printf("Finished writing **&&, size of write: %d, should be %d\n", wret, 4);
		fflush(stdout);
	}
	
}

int main(int argc, char** argv){
        pid_t   childid;
	float freq = 1;
	float salt = 0.0;
	float svel = 100.0;
	float slat = 45.0;
	float slon = 45.0;
	int i=0;
	int piperet=0;
	char* buffsize = NULL;;
//	char** args;
	int ret;
	//int file;
	int child_exit = -5;
	debug = 0;
	name = NULL;
//	args = (char**)malloc(sizeof(char*)*1);
//	args[0] = (char*)malloc(sizeof(char)*7);
//	memset(args[0], '\0', 7);
//	strcpy(args[0], "-debug");
	srand(time(NULL));
//	file = -5;
	for(i; i<argc; i++){
		if(strcmp(argv[i], "-vel") == 0){ //get starting velocity
			svel = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-alt") == 0){ //get starting altitude
			salt = atof(argv[i+1]);
		}
		//else if(strcmp(argv[i], "-to") == 0){ //get starting altitude
			//printf("to set\n");
		//	file = 1;
		//}
		else if(strcmp(argv[i], "-lat") == 0){ //get starting latitude
			slat = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-lon") == 0){ //get starting longitude
			slon = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-rate") == 0){ //get update rate in Hz
			freq = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-buff") == 0){ //get read buffer size for logger
			buffsize = argv[i+1];
		}
		else if(strcmp(argv[i], "-debug") == 0){ //get update rate in Hz
			debug = 1;;
			printf("data_gen debug: on\n");
			fflush(stdout);
		}
	}
	//piperet = pipe(files);
	//if(piperet == -1){
	//	printf("ERROR MAKING PIPE\n");
	//	fflush(stdout);
	//	exit(1);
	//}
	if(debug){
			printf("data_gen making fifo\n");
			fflush(stdout);
	}
	mkfifo(fifoname, 0666);//make a FIFO
	//fifo = open("./commfifo", O_WRONLY);
	if(debug){
			printf("data_gen fifo made\n");
			fflush(stdout);
	}
//	usleep(1000);
//	fifo = open(fifoname, O_WRONLY);
	if(debug){
			printf("data_gen spawning child\n");
			fflush(stdout);
	}
	
	childid = fork();

	if(debug){
			printf("data_gen child spawned\n");
			fflush(stdout);
	}

	switch(childid){

	case -1: //error
		printf("ERROR SPAWNING CHILD\n");
		fflush(stdout);
		exit(2);

	case 0: //child
                /* remap stdin (0) to the read pipe */
		//close(files[1]); //close write
                //dup2(3, files[0]); //duplicate read side to stdin
		printf("starting handle as child process\n");
		fflush(stdout);
               	ret = execlp("python", "python", "./handle.py", NULL);
		if(ret == -1){
			printf("ERROR LAUNCHING LOGGER\n");
			fflush(stdout);
			exit(3);
		}
                //execlp("../logger", "../logger", args);

        default: //parent
		//close(files[0]); //close read
		if(debug){
			printf("gen_data opening fifo\n");
			fflush(stdout);
		}
		//fifo = open(fifoname, O_WRONLY);
		if(debug){
			printf("gen_data fifo open\n");
			printf("staring gen_data in generator process\n");
			fflush(stdout);
		}
		gen_data(freq, salt, svel, slat, slon);
		waitpid(childid, &child_exit, 0);
		//close(fifo);
	}
	

	childid = fork();
	
	switch(childid){
	
	case -1: //error
		printf("ERROR SPAWNING CHILD\n");
		fflush(stdout);
		exit(2);
	case 0: //child
		printf("data_gen removing fifo\n");
		fflush(stdout);
               	ret = execlp("rm", "rm", fifoname, NULL); //remove fifo
		if(ret == -1){
			printf("ERROR LAUNCHING RM\n");
			fflush(stdout);
			exit(3);
		}
        default: //parent
		waitpid(childid, &child_exit, 0); //wait on child
	}
	return 0;

}
