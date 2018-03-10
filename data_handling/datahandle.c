#define _XOPEN_SOURCE 500 //makes usleep work
#include <conversion.h>
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
#include "datagen.h"

//communication globals
int files1[2];
int files2[2];
int fifo;
pid_t children[3];
int childcnt=0;


void get_data2(char* input, int size){
	memset(input, '\0', size);
	get_data(input);
	
}

int spawn_raw_log(){
	pid_t idret; //id return
	int execret = -5; //return from exec

	//spawn child
	idret = fork();
	switch(idret){
		case -1: //error
			printf("ERROR SPAWNING RAW LOG\n");
			exit(1);
			break;
		case 0: //child
			//exec into raw logger
			close(files1[1]);//close write
			dup2(files1[0], 3); //remap pipe read to file 3
                        execret = execlp("../logging/logger", "../logging/logger", "-out", "rawlog.txt",  (char*)NULL);
                        if(execret == -1){//exec error
                                printf("ERROR LAUNCHING %s", "RAW LOGGER");
                        	exit(3);
                        }
			break;
		default: //parent
                        close(files1[0]);//close read
			children[childcnt] = idret; //store child pid
			printf("rawlog: %d\n", idret);
			childcnt++; //increment child count
			break;
	}
	return idret;
}

int spawn_json_log(){
        pid_t idret; //id return
        int execret = -5; //return from exec

        //spawn child
        idret = fork();
        switch(idret){
                case -1: //error
                        printf("ERROR SPAWNING JSON LOG\n");
                        exit(1);
                        break;
                case 0: //child
                        //exec into JSON logger
                        close(files2[1]);//close write
                        dup2(files2[0], 3); //remap pipe read to file 3
                        execret = execlp("../logging/logger", "../logging/logger", "-out", "jsonlog.txt",  (char*)NULL);
                        if(execret == -1){//exec error
                                printf("ERROR LAUNCHING %s", "JSON LOGGER");
                                exit(3);
                        }
                        break;
                default: //parent
                        close(files2[0]);//close read
			children[childcnt] = idret; //store child pid
			printf("jsonlog: %d\n", idret);
			childcnt++; //increment child count
                        break;
        }
        return idret;

}

int spawn_api_handle(){
        pid_t idret; //id return
        int execret = -5; //return from exec

        //spawn child
        idret = fork();
        switch(idret){
                case -1: //error
                        printf("ERROR SPAWNING RAW LOG\n");
                        exit(1);
                        break;
                case 0: //child
                        //exec into api handler
                        //close(files1[1]);//close write
                        //dup2(3, files1[0]); //remap pipe read to file 3
                        execret = execlp("python", "python", "./handle.py",(char*)NULL);
                        if(execret == -1){//exec error
                                printf("ERROR LAUNCHING %s", "RAW LOGGER");
                                exit(3);
                        }
                        break;
                default: //parent
			children[childcnt] = idret; //store child pid
			printf("apihandle: %d\n", idret);
			childcnt++; //increment child count
                        break;
        }
        return idret;

}

int spawn_trace(){

}

void remove_fifo(){
	pid_t childid;
	int child_exit = -5;
	int ret;

	childid = fork();

	switch(childid){
		case -1: //error
			printf("ERROR SPAWNING CHILD\n");
			fflush(stdout);
			exit(2);
		case 0: //child
			ret = execlp("rm", "rm", "./commfifo", NULL); //remove fifo
			if(ret == -1){
				printf("ERROR LAUNCHING RM\n");
				fflush(stdout);
				exit(4);
			}
		default: //parent
			waitpid(childid, &child_exit, 0); //wait on child
	}
}

void write_to_raw_log(char* input){
	int total = 0;
	int wret = 0;
	char* tmp;
	tmp = input;
	while(total < strlen(input)){ //while not finished writing
		printf("datahandle writing to raw: %s\n", tmp);
		wret = write(files1[1], tmp, (strlen(tmp))); //write packet
		tmp += wret; //shorten message accordingly 
		total += wret; //track total written 
	}
}

void write_to_json_log(char* input){
	int total = 0;
	int wret = 0;
	char* tmp;
	tmp = input;
	while(total < strlen(input)){ //while not finished writing
		printf("datahandle writing to json: %s\n", tmp);
		wret = write(files2[1], tmp, (strlen(tmp))); //write packet
		tmp += wret; //shorten message accordingly 
		total += wret; //track total written 
	}
}

int main(int argc, char** argv){
	pid_t raw_logid, json_logid, api_handleid, traceid; //process id's
	int child_exit = -5; //child exit status bucket
	int piperet1; //pipe return
	int piperet2; //pipe return
	int fiforet; //fifo return
	int execret; //exec return
	int size = 256; 
	char retrieved_data[size]; //data string
        int i=0; //common iterator variable 
        char* buffsize = NULL;
        int ret; //return from exec
	int j = 0; //another iterator
	char end[5] = "**&&";
        wind = -5;
        debug = 0; //default debug off
        name = NULL;
        srand(time(NULL)); //seed random
        for(i; i<argc; i++){
                if(strcmp(argv[i], "-vel") == 0){ //get starting velocity
                        vel = atof(argv[i+1]);
                }
                else if(strcmp(argv[i], "-alt") == 0){ //get starting altitude
                        alt = atof(argv[i+1]);
                }
                else if(strcmp(argv[i], "-lat") == 0){ //get starting latitude
                        lat = atof(argv[i+1]);
                }
                else if(strcmp(argv[i], "-lon") == 0){ //get starting longitude
                        lon = atof(argv[i+1]);
                }
                else if(strcmp(argv[i], "-wind") == 0){ //get starting longitude
                        wind = 1; //east
                }
                else if(strcmp(argv[i], "-rate") == 0){ //get update rate in Hz
                        freq = atof(argv[i+1]);
                }
                else if(strcmp(argv[i], "-buff") == 0){ //get read buffer size for logger
                        buffsize = argv[i+1];
                }
                else if(strcmp(argv[i], "-test") == 0){ //get read buffer size for logger
                        srand(5); //seed random
                }
                else if(strcmp(argv[i], "-debug") == 0){ //get update rate in Hz
                        debug = 1;
                        printf("data_gen debug: on\n");
                        fflush(stdout);
                }
        }
	
	salt = alt; //remember initial params
	svel = vel;
	slat = lat;
	slon = lon;
	

	//making communication pipe for raw log
	piperet1 = pipe(files1);
        if(piperet1 == -1){
        	printf("ERROR MAKING PIPE\n");
        	fflush(stdout);
        	exit(2);
        }
	//making communication pipe for json log
	piperet2 = pipe(files2);
        if(piperet2 == -1){
        	printf("ERROR MAKING PIPE\n");
        	fflush(stdout);
        	exit(2);
        }

	//making communication fifo for api handler
	fiforet = mkfifo("./commfifo", 0666);//make a FIFO
        if(fiforet == -1){
                printf("ERROR MAKING FIFO\n");
                fflush(stdout);
                exit(3);
        }



	//spawn processes 	
	raw_logid = spawn_raw_log();
	json_logid = spawn_json_log();
	api_handleid = spawn_api_handle();
	//traceid = spawn_trace();
	
	//start loop
//	while(1){
	do{
		printf("in loop\n");
		get_data2(retrieved_data, size);//get data from hardware
		printf("in main data is: %s\n", retrieved_data);
		//raw log
		write_to_raw_log(retrieved_data);
		//convert to json
		//interpolate
		//json log
		write_to_json_log(retrieved_data);
		//database
		//trace
		
	}while(alt > salt);	
	printf("out of loop\n");
//	}
	usleep(1000); //sleep fpr a little while
	write_to_raw_log(end);
	write_to_json_log(end);

	printf("waiting on children\n");
	i=0;
	for(i; i<childcnt; i++){//clean up children
		printf("waiting on: %d\n", children[i]);
		waitpid(children[i], &child_exit, 0);
	}
	remove_fifo();
	printf("finished waiting, exiting\n");
	return 0;
}
