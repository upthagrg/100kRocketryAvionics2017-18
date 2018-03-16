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
int files1_1[2];
int files1_2[2];
int files2_1[2];
int files2_2[2];
int fifo;
pid_t children[3];
int childcnt=0;

void get_data2(char* input, int size){
	memset(input, '\0', size);
	get_data(input);
	
}

int spawn_raw_log(int in){
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
			if(in == 1){
				close(files1_1[1]);//close write
				dup2(files1_1[0], 3); //remap pipe read to file 3
			}
			else if(in == 2){
				close(files1_2[1]);//close write
				dup2(files1_2[0], 3); //remap pipe read to file 3
			}
			else{
				printf("Error, invalid pipe\n");
				exit(8);
			}
			if(debug){
				if(in == 1){
                        		execret = execlp("../logging/logger", "../logging/logger", "-out", "rawlog1.txt", "-debug",  (char*)NULL);
				}
				else if(in == 2){
                        		execret = execlp("../logging/logger", "../logging/logger", "-out", "rawlog2.txt", "-debug",  (char*)NULL);
				}
			}
			else{
				if(in == 1){
                        		execret = execlp("../logging/logger", "../logging/logger", "-out", "rawlog1.txt",  (char*)NULL);
				}
				else if(in == 2){
                        		execret = execlp("../logging/logger", "../logging/logger", "-out", "rawlog2.txt",  (char*)NULL);
				}
			}
                        if(execret == -1){//exec error
                                printf("ERROR LAUNCHING %s", "RAW LOGGER");
                        	exit(3);
                        }
			break;
		default: //parent
			if(in == 1){
                        	close(files1_1[0]);//close read
			}
			else{
	                        close(files1_2[0]);//close read
			}
			children[childcnt] = idret; //store child pid
			if(debug){
				//print pid
				printf("rawlog: %d\n", idret);
			}
			childcnt++; //increment child count
			break;
	}
	return idret;
}

int spawn_json_log(int in){
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
			if(in == 1){
                        	close(files2_1[1]);//close write
                        	dup2(files2_1[0], 3); //remap pipe read to file 3
			}
			else{
                        	close(files2_2[1]);//close write
                        	dup2(files2_2[0], 3); //remap pipe read to file 3
			}
			if(debug){
				if(in == 1){
                        		execret = execlp("../logging/logger", "../logging/logger", "-out", "jsonlog1.txt", "-debug",  (char*)NULL);
				}
				else if(in == 2){
                        		execret = execlp("../logging/logger", "../logging/logger", "-out", "jsonlog2.txt", "-debug",  (char*)NULL);
				}
			}
			else{
				if(in == 1){
                        		execret = execlp("../logging/logger", "../logging/logger", "-out", "jsonlog1.txt", (char*)NULL);
				}
				else if(in == 2){
                        		execret = execlp("../logging/logger", "../logging/logger", "-out", "jsonlog2.txt", (char*)NULL);
				}
			}
                        if(execret == -1){//exec error
                                printf("ERROR LAUNCHING %s", "JSON LOGGER");
                                exit(3);
                        }
                        break;
                default: //parent
			if(in ==1){
                        	close(files2_1[0]);//close read
			}
			else{
                        	close(files2_2[0]);//close read
			}
			children[childcnt] = idret; //store child pid
			if(debug){
				//print pid
				printf("jsonlog: %d\n", idret);
			}
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
			if(debug){
				//print pid
				printf("pihandle: %d\n", idret);
			}
			childcnt++; //increment child count
                        break;
        }
        return idret;

}

int spawn_trace(){

}

void remove_fifo(int in){
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
			switch(in){
				case 1:
					ret = execlp("rm", "rm", "./commfifo1", NULL); //remove fifo
				case 2:
					ret = execlp("rm", "rm", "./commfifo2", NULL); //remove fifo
				default:
					ret = -1;
			}
			if(ret == -1){
				printf("ERROR LAUNCHING RM\n");
				fflush(stdout);
				exit(4);
			}
		default: //parent
			waitpid(childid, &child_exit, 0); //wait on child
	}
}

void write_to_raw_log(char* input, int output){
	int total = 0;
	int wret = 0;
	char* tmp;
	tmp = input;
	while(total < strlen(input)){ //while not finished writing
		if(debug){
			printf("Datahandle writing to raw: %s\n", tmp);
		}
		switch(output){
			case 1:
				wret = write(files1_1[1], tmp, (strlen(tmp))); //write packet
			case 2:
				wret = write(files1_2[1], tmp, (strlen(tmp))); //write packet
			default:
				printf("Error, invalid pipe\n");
				exit(8);
		}
		tmp += wret; //shorten message accordingly 
		total += wret; //track total written 
	}
}

void write_to_json_log(char* input, int output){
	int total = 0;
	int wret = 0;
	char* tmp;
	tmp = input;
	while(total < strlen(input)){ //while not finished writing
		if(debug){
			printf("Datahandle writing to json: %s\n", tmp);
		}
		switch(output){
			case 1:
				wret = write(files2_1[1], tmp, (strlen(tmp))); //write packet
			case 2:
				wret = write(files2_2[1], tmp, (strlen(tmp))); //write packet
			default:
				printf("Error, invalid pipe\n");
				exit(8);
		}
		tmp += wret; //shorten message accordingly 
		total += wret; //track total written 
	}
}

int main(int argc, char** argv){
	pid_t raw_logid1, json_logid1, raw_logid2, json_logid2, api_handleid, traceid; //process id's
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
	piperet1 = pipe(files1_1);
        if(piperet1 == -1){
        	printf("ERROR MAKING PIPE\n");
        	fflush(stdout);
        	exit(2);
        }
	//making communication pipe for raw log
	piperet1 = pipe(files1_2);
        if(piperet1 == -1){
        	printf("ERROR MAKING PIPE\n");
        	fflush(stdout);
        	exit(2);
        }
	//making communication pipe for json log
	piperet2 = pipe(files2_1);
        if(piperet2 == -1){
        	printf("ERROR MAKING PIPE\n");
        	fflush(stdout);
        	exit(2);
        }
	//making communication pipe for json log
	piperet2 = pipe(files2_2);
        if(piperet2 == -1){
        	printf("ERROR MAKING PIPE\n");
        	fflush(stdout);
        	exit(2);
        }

	//making communication FIFOs for api handler
	fiforet = mkfifo("./commfifo1", 0666);//make a FIFO
        if(fiforet == -1){
                printf("ERROR MAKING FIFO\n");
                fflush(stdout);
                exit(3);
        }
	
	fiforet = mkfifo("./commfifo2", 0666);//make a FIFO
        if(fiforet == -1){
                printf("ERROR MAKING FIFO\n");
                fflush(stdout);
                exit(3);
        }



	//spawn processes 	
	raw_logid1 = spawn_raw_log(1);
	raw_logid2 = spawn_raw_log(2);
	json_logid1 = spawn_json_log(1);
	json_logid2 = spawn_json_log(2);
	//api_handleid = spawn_api_handle();
	//traceid = spawn_trace();
	
	//start loop
//	while(1){
	do{
		if(debug){
			//inform user that control flow is at top of loop
			printf("In loop\n");
		}
		get_data2(retrieved_data1, retrieved_data2, size);//get data from hardware
		if(debug){
			//print out retrived data
			printf("In main, data is: %s\n", retrieved_data1);
			printf("In main, data is: %s\n", retrieved_data2);
		}
		//raw log
		write_to_raw_log(retrieved_data1, 1);
		write_to_raw_log(retrieved_data2, 2);
		//convert to json
		//interpolate
		//json log
		write_to_json_log(retrieved_data1, 1);
		write_to_json_log(retrieved_data2, 2);
		//database
		//trace
		
	}while(alt > salt);
	if(debug){
		//inform user that loop has ended, aka end of transmit	
		printf("Out of loop\n");
	}
//	}
	usleep(1000); //sleep fpr a little while
	write_to_raw_log(end);
	write_to_json_log(end);

	printf("Waiting on children...\n");
	i=0;
	for(i; i<childcnt; i++){//clean up children
		if(debug){
			//print pid being closed 
			printf("Waiting on: %d\n", children[i]);
		}
		waitpid(children[i], &child_exit, 0);
	}
	printf("Finished waiting\n");
	printf("Removing FIFOs...\n");
	remove_fifo(1);
	remove_fifo(2);
	printf("Finished removing FIFOs\n");
	printf("Exiting\n");
	return 0;
}
