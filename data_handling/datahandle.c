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
#include <pthread.h>
#include "datagen.h"

//communication globals
int files1_1[2];
int files1_2[2];
int files2_1[2];
int files2_2[2];
int fifo1;
int fifo2;
pid_t children[5];
int childcnt=0;
int debug2=0;
//globals for threading
char latest_packet[256];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int run = 0;

void remove_fifo(int);

void get_data2(char* input, char* input2, int size){
	memset(input, '\0', size);
	memset(input2, '\0', size);
	get_data(input);
	get_data_2(input2);
	//strcpy(input2, input);
	
}

int start_docker(){
	pid_t idret = -5; //id return 
	int execret = -5; //return from exec
	int child_exit;
	idret = fork();
	switch(idret){
		case -1: //error
			printf("ERROR SPAWNING RAW LOG\n");
			remove_fifo(1);
			remove_fifo(2);
			exit(1);
			break;
		case 0:
                        execret = execlp("./start_docker.sh", "./start_docker.sh", (char*)NULL);
		default:
			waitpid(idret, &child_exit, 0);
			break;
	}
	return idret;
}

int spawn_raw_log(int in){
	pid_t idret; //id return
	int execret = -5; //return from exec

	//spawn child
	idret = fork();
	switch(idret){
		case -1: //error
			printf("ERROR SPAWNING RAW LOG\n");
			remove_fifo(1);
			remove_fifo(2);
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
				printf("Error, invalid pipe instance:1, input: %d\n",in);
				remove_fifo(1);
				remove_fifo(2);
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
				remove_fifo(1);
				remove_fifo(2);
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
			remove_fifo(1);
			remove_fifo(2);
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
				remove_fifo(1);
				remove_fifo(2);
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
			remove_fifo(1);
			remove_fifo(2);
                        exit(1);
                        break;
                case 0: //child
                        //exec into api handler
                        //close(files1[1]);//close write
                        //dup2(3, files1[0]); //remap pipe read to file 3
                        execret = execlp("python", "python", "./handle.py",(char*)NULL);
                        if(execret == -1){//exec error
                                printf("ERROR LAUNCHING %s", "RAW LOGGER");
				remove_fifo(1);
				remove_fifo(2);
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
			if(output == 1){
				wret = write(files1_1[1], tmp, (strlen(tmp))); //write packet
			}
			else if(output == 2){
				wret = write(files1_2[1], tmp, (strlen(tmp))); //write packet
			}
			else{
				printf("Error, invalid pipe, instance:2 input: %d\n", output);
				remove_fifo(1);
				remove_fifo(2);
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
			if(output == 1){
				wret = write(files2_1[1], tmp, (strlen(tmp))); //write packet
			}
			else if(output == 2){
				wret = write(files2_2[1], tmp, (strlen(tmp))); //write packet
			}
			else{
				printf("Error, invalid pipe instance:3, input: %d\n", output);
				remove_fifo(1);
				remove_fifo(2);
				exit(8);
			}
		tmp += wret; //shorten message accordingly 
		total += wret; //track total written 
	}
}

void write_to_api(char* input, int output){
        int total = 0;
        int wret = 0;
        char* tmp;
        tmp = input;
        while(total < strlen(input)-2){ //while not finished writing
		if(output == 1){
			fifo1 = open("./commfifo1",  O_WRONLY);
		}
		else{
			fifo2 = open("./commfifo2",  O_WRONLY);
		}
                if(debug2){
                        printf("Datahandle writing to api: %s\n", tmp);
                }
                        if(output == 1){
                                wret = write(fifo1, tmp, (strlen(tmp)-2)); //write packet
                        }
                        else if(output == 2){
                                wret = write(fifo2, tmp, (strlen(tmp)-2)); //write packet
                        }
                        else{
                                printf("Error, invalid pipe instance:3, input: %d\n", output);
                                remove_fifo(1);
                                remove_fifo(2);
                                exit(8);
                        }
                tmp += wret; //shorten message accordingly 
                total += wret; //track total written 
		if(output == 1){
			close(fifo1);
		}
		else{
			close(fifo2);
		}
        }
}

void* _trace_update(){
	while(run == 0){
		continue;
	}
	while(run == 1){	
		pthread_mutex_lock(&lock);
		printf("TRACE THREAD HAS: %s\n", latest_packet);
		fflush(stdout);
		pthread_mutex_unlock(&lock);
		usleep(1000000/1);	
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
	char retrieved_data1[size]; //data string
	char retrieved_data2[size]; //data string
        int i=0; //common iterator variable 
        char* buffsize = NULL;
        int ret; //return from exec
	int j = 0; //another iterator
	char end[5] = "**&&";
	int ended1 = 0;
	int ended2 = 0;
	pthread_t trace_com;
        wind = -5;
        debug = 0; //default debug off
        name = NULL;
        srand(time(NULL)); //seed random
	//lock = PTHREAD_MUTEX_INITIALIZER; //initialize lock
        for(i; i<argc; i++){
                if(strcmp(argv[i], "-vel") == 0){ //get starting velocity
                        vel = atof(argv[i+1]);
                        vel2 = atof(argv[i+1]);
                }
                else if(strcmp(argv[i], "-alt") == 0){ //get starting altitude
                        alt = atof(argv[i+1]);
                        alt2 = atof(argv[i+1]);
                }
                else if(strcmp(argv[i], "-lat") == 0){ //get starting latitude
                        lat = atof(argv[i+1]);
                        lat2 = atof(argv[i+1]);
                }
                else if(strcmp(argv[i], "-lon") == 0){ //get starting longitude
                        lon = atof(argv[i+1]);
                        lon2 = atof(argv[i+1]);
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
                else if(strcmp(argv[i], "-debug2") == 0){ //get update rate in Hz
                        debug2 = 1;
                        printf("data_gen debug: on\n");
                        fflush(stdout);
                }
        }


	printf("\n\nstarting docker...\n\n");
	start_docker();
	printf("\n\ndocker up\n\n");
	
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
	api_handleid = spawn_api_handle();
	//traceid = spawn_trace();
	
	//create thread to talk to trace
//	pthread_create(&trace_com, NULL, _trace_update, NULL);

	//start loop
//	while(1){
	do{
		if(debug){
			//inform user that control flow is at top of loop
			printf("In loop\n");
		}
		get_data2(retrieved_data1, retrieved_data2, size);//get data from hardware
		run = 1;
		//update latest_packet
		pthread_mutex_lock(&lock);
		memset(latest_packet, '\0', 256);
		strcpy(latest_packet, retrieved_data1);
		printf("latest packet: %s\n", latest_packet);
		pthread_mutex_unlock(&lock);
		if(debug){
			//print out retrived data
			printf("In main, data is: %s\n", retrieved_data1);
			printf("In main, data is: %s\n", retrieved_data2);
		}
		//raw log
		if(retrieved_data1[0] != 'E'){
			write_to_raw_log(retrieved_data1, 1);
		}
		if(retrieved_data2[0] != 'E'){
			write_to_raw_log(retrieved_data2, 2);
		}
		//convert to json
		//interpolate
		//json log
		if(retrieved_data1[0] != 'E'){
			write_to_json_log(retrieved_data1, 1);
		}
		if(retrieved_data2[0] != 'E'){
			write_to_json_log(retrieved_data2, 2);
		}
		//write to API, leading to database
		if(retrieved_data1[0] != 'E'){
			printf("writing to api1: %s\n", retrieved_data1);
			write_to_api(retrieved_data1, 1);
			printf("finished writing to api1\n");
		}
		else{
			if(ended1 == 0){
				printf("writing EOT to api1\n");
				write_to_api(end, 1);
				printf("done\n");
				ended1 = 1;
			}
		}
		if(retrieved_data2[0] != 'E'){
			printf("writing to api2: %s\n", retrieved_data2);
			write_to_api(retrieved_data2, 2);
			printf("finished writing to api2\n");
		}
		else{
			if(ended2 == 0){
				printf("writing EOT to api2\n");
				write_to_api(end, 2);
				printf("done\n");
				ended2 = 1;
			}
		}
		//trace
	//	printf("alt: %f, alt2: %f\n", alt, alt2);
	}while(alt2 > salt);
	if(debug){
		//inform user that loop has ended, aka end of transmit	
		printf("Out of loop\n");
	}
//	}
	usleep(1000); //sleep fpr a little while
	write_to_raw_log(end,1);
	write_to_raw_log(end,2);
	write_to_json_log(end,1);
	write_to_json_log(end,2);
	if(ended1 == 0){
		write_to_api(end, 1);
	}
	if(ended2 == 0){
		write_to_api(end, 2);
	}

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
	printf("Joining threads...\n");
//	pthread_join(trace_com, NULL);
	printf("Threads joined\n");
	printf("Removing FIFOs...\n");
	remove_fifo(1);
	remove_fifo(2);
	printf("Finished removing FIFOs\n");
	printf("Exiting\n");
	return 0;
}
