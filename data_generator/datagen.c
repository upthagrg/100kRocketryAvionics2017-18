/*******************************************************************************************
*Title: datagen.c
*Author: Glenn Upthagrove
*Date: 01/29/2018
*tracking software. The altitude is a simple quadratic, and the velocity is the derivative
*thereof. The rate of update can be set via -rate, the initial altitude via -alt, the 
*initial latitude via -lat, the initial longitude via -lon, and the initial velocity via 
*-vel. If any of thee are not set, defaults are used. Use -pipe to use a pipe followed by 
*the name of the process and all of its arguments. The same goes for a FIFO. Use -pipeno to
*set an FD number to use for the pipe in the child process. You cannot use a pipe and FIFO
*at the same time. 
*******************************************************************************************/
#define _XOPEN_SOURCE 500 //makes usleep work
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <conversion.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


int file;
int fifo;
int mypipe;
int files[2];
int fifo_file;
char* fifoname = "./commfifo";
int debug;
char* name;
int pipeno;
int myfile;
int wind;
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
		if(mypipe){
			if(debug){
				printf("PIPE DETECTED\n");
			}
			sprintf(buff,"{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\"}&&", str1,vel,str2,lat,str3,lon,str4,alt,str5,time); //make JSON string
		}
		else{
			if(debug){
				printf("PIPE NOT DETECTED\n");
			}
			sprintf(buff,"{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\"}", str1,vel,str2,lat,str3,lon,str4,alt,str5,time); //make JSON string
		}
		//fprintf(files[1], buff); //print JSON string
		//fprintf(files[1], "\n"); //print newline
		buff2 = buff;
		while(total < strlen(buff2)){ //loop until total message is written
			if(fifo){ //if writing to fifo, open fifo
				fifo_file = open(fifoname, O_WRONLY);
			}
			if(debug){
				printf("data_gen writing\n");
				fflush(stdout);
				printf("%s\n", buff2);
				fflush(stdout);
			}

			if(fifo){//if writing to fifo
				wret = write(fifo_file, buff2, (strlen(buff2))); //write to FIFO
			}
			else if(file){//if writing to file
				wret = write(myfile, buff2, (strlen(buff2)));
				write(myfile, "\n", 1);
			}
			else{ //using pipe
				wret = write(files[1], buff2, (strlen(buff2)));
			}

			if(debug){
				printf("data_gen finished writing\n");
				fflush(stdout);
			}
			if(wret == -1){
				printf("WRITE ERROR IN GEN_DATA\n");
				if(fifo){
					printf("USING FIFO\n");	
				}
				else if(mypipe){
					printf("USING A PIPE\n");
				}
				else{
					printf("USING FILE\n");
				}
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
		if(fifo){ //if using a fifo, close fifo
			close(fifo_file);
		}
		//update variables
		time += (1.0/freq); //update time
		alt = ((-4.9*powf(time, 2)) + (svel*time) + salt); //update alt
		dec = rand()%2;
		if(dec){ //update lat
			lat += 0.0001;
		}
		else{
			lat -= 0.0001;
		}
		dec = rand()%2;
		if(dec){ //update lon
			lon += 0.00017;
		}
		else{
			lon -= 0.00017;
		}
		if(wind == 1){
			lon += 0.00001; //east
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
	if(fifo){
		fifo_file = open(fifoname, O_WRONLY);
		wret = write(fifo_file, "**&&", 4);
		close(fifo_file);
	}
	else if(mypipe){
		wret = write(files[1], "**&&", 4);
	}
	else{
		wret = write(1, "**&&", 4);
		wret = write(1, "\n", 1);
	}
	if(debug){
		printf("Finished writing **&&, size of write: %d, should be %d\n", wret, 4);
		fflush(stdout);
	}
}

int main(int argc, char** argv){
        pid_t   childid; //id of spawned child
	float freq = 1;
	float salt = 0.0;
	float svel = 100.0;
	float slat = 45.0;
	float slon = 45.0;
	int i=0; //common iterator variable 
	int piperet=0; //return from pipe
	char* buffsize = NULL;
	int ret; //return from exec
	int child_exit = -5; //child process exit status
	char* proc_name = NULL; //name of process to launch
	char** args; //set of arguments for launched process
	int pos; //position for grabbing arguments
	int minus=2; //for figuring out argument stripping
	wind = -5;
	pipeno = 3; //default use fd3
	mypipe = 0; //default no pipe
	fifo = 0; //default no fifo
	file = 0; //default no file
	debug = 0; //default debug off
	name = NULL;
	srand(time(NULL)); //seed random
	myfile = 1; //initialize to stdout
	for(i; i<argc; i++){
		if(strcmp(argv[i], "-vel") == 0){ //get starting velocity
			svel = atof(argv[i+1]);
			minus++;
			minus++;
		}
		else if(strcmp(argv[i], "-alt") == 0){ //get starting altitude
			salt = atof(argv[i+1]);
			minus++;
			minus++;
		}
		else if(strcmp(argv[i], "-lat") == 0){ //get starting latitude
			slat = atof(argv[i+1]);
			minus++;
			minus++;
		}
		else if(strcmp(argv[i], "-lon") == 0){ //get starting longitude
			slon = atof(argv[i+1]);
			minus++;
			minus++;
		}
		else if(strcmp(argv[i], "-wind") == 0){ //get starting longitude
			wind = 1; //east
			minus++;
		}
		else if(strcmp(argv[i], "-rate") == 0){ //get update rate in Hz
			freq = atof(argv[i+1]);
			minus++;
			minus++;
		}
		else if(strcmp(argv[i], "-buff") == 0){ //get read buffer size for logger
			buffsize = argv[i+1];
			minus++;
			minus++;
		}
		else if(strcmp(argv[i], "-to") == 0){ //get read buffer size for logger
			myfile = open(argv[i+1], O_WRONLY|O_CREAT, 0644); //open specified file
			minus++;
			minus++;
			file = 1;
		}
		else if(strcmp(argv[i], "-test") == 0){ //get read buffer size for logger
			srand(5); //seed random
			minus++;
		}
		else if(strcmp(argv[i], "-debug") == 0){ //get update rate in Hz
			debug = 1;
			printf("data_gen debug: on\n");
			fflush(stdout);
			minus++;
		}
		else if(strcmp(argv[i], "-pipe") == 0){ //use a pipe, get process to spawn
			proc_name = argv[i+1];
			mypipe = 1;
			pos = i+1;
			break;
		}
		else if(strcmp(argv[i], "-pipeno") == 0){ //use a pipe, get pipe number
			pipeno = atoi(argv[i+1]);
		}
		else if(strcmp(argv[i], "-fifo") == 0){ //use a fifo, get process to spawn
			proc_name = argv[i+1];
			fifo = 1;
			pos = i+1;
			break;
		}
	}
	if(mypipe || fifo){
		//get args for execvp
		printf("minus: %d", minus);
		args = (char**)malloc(sizeof(char*)*(argc-pos+1));
		i=0;
		for(i; i<argc-minus; i++){
			args[i] = (char*)malloc(sizeof(char)*64);
			memset(args[i], '\0', 64);
			strcpy(args[i], argv[i+pos]);
		}
		args[argc-pos] = NULL; // set last in array to NULL
	}
	if(!mypipe && !fifo){ //pipe and fifo both not set, thus writing to file
		file = 1; 
	}
	if(mypipe && fifo){
		printf("CANNOT USE PIPE AND FIFO AT SAME TIME\n");
		exit(7);
	}

	if(mypipe){ //using a pipe
		piperet = pipe(files);
		if(piperet == -1){
			printf("ERROR MAKING PIPE\n");
			fflush(stdout);
			exit(1);
		}

		childid = fork();

		switch(childid){
			case -1: //error
				printf("ERROR SPAWNING CHILD\n");
              			fflush(stdout);
                		exit(2);
			case 0: //child
				/* remap pipeno fd to the read pipe  and exec*/
                		close(files[1]); //close write
                		dup2(pipeno, files[0]); //duplicate read side to pipeno (default 3)
                		dup2(files[0], pipeno); //duplicate read side to pipeno (default 3)
				if(debug){
					printf("starting %s as child process\n", proc_name);
                        		fflush(stdout);
				}
				ret = execvp(proc_name, args);
				if(ret == -1){
					printf("ERROR LAUNCHING %s", proc_name);
					exit(3);
				}
			default: //parent
				close(files[0]); //close read
		                if(debug){
		                        printf("staring gen_data in generator process\n");
		                        fflush(stdout);
		                }
		                gen_data(freq, salt, svel, slat, slon);
		                waitpid(childid, &child_exit, 0);
		}
	}

	else if(fifo){ //using a fifo
		if(debug){
			printf("data_gen making fifo\n");
			fflush(stdout);
		}
		mkfifo(fifoname, 0666);//make a FIFO
		if(debug){
			printf("data_gen fifo made\n");
			fflush(stdout);
		}
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
			if(debug){
				printf("starting %s as child process\n", proc_name);
				fflush(stdout);
			}
		        
			ret = execvp(proc_name, args);
			
			if(ret == -1){
				printf("ERROR LAUNCHING %s\n", proc_name);
				fflush(stdout);
				exit(3);
			}
	
	        default: //parent
			if(debug){
				printf("staring gen_data in generator process\n");
				fflush(stdout);
				}
			gen_data(freq, salt, svel, slat, slon);
			waitpid(childid, &child_exit, 0);
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
	}

	else{ // writing to file
		gen_data(freq, salt, svel, slat, slon);
	}
	if(myfile > 2){
		close(myfile);//close the file
	}
	return 0;

}
