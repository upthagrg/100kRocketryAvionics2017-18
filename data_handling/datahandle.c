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

//communication globals
int files1[2];
int files2[2];
int fifo;

void get_data(char* input, int size){
	memset(input, '\0', size);
}

int spawn_raw_log(){
	int idret; //id return
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
			dup2(3, files1[0]); //remap pipe read to file 3
			execret = execlp("../logging/logger", "../logging/logger", (char*)NULL);
                        if(execret == -1){//exec error
                                printf("ERROR LAUNCHING %s", "RAW LOGGER");
                        	exit(3);
                        }
			break;
		default: //parent
			break;
	}
	return idret;
}

int spawn_json_log(){
        int idret; //id return
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
                        dup2(3, files2[0]); //remap pipe read to file 3
                        execret = execlp("../logging/logger", "../logging/logger", (char*)NULL);
                        if(execret == -1){//exec error
                                printf("ERROR LAUNCHING %s", "JSON LOGGER");
                                exit(3);
                        }
                        break;
                default: //parent
                        break;
        }
        return idret;

}

int spawn_api_handle(){
        int idret; //id return
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

int main(){
	pid_t raw_logid, json_logid, api_handleid, traceid; //process id's
	int child_exit = -5; //child exit status bucket
	int piperet1; //pipe return
	int piperet2; //pipe return
	int fiforet; //fifo return
	int execret; //exec return
	int size = 256; 
	char retrieved_data[size]; //data string
	
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
	while(1){
		get_data(retrieved_data, size);//get data from hardware
		
	}

	return 0;
}
