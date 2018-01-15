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
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int compare(){
	char* buffer;
	char* buffer2;
	int text_file, bytes;
	struct stat bucket;
	char* name;
	char* name2;
	name = "testlog.txt";
	name2 = "log.txt";
	//printf("opening file\n");
	text_file = open(name, O_RDONLY);
	stat(name, &bucket);
	buffer = malloc((sizeof(char))*(bucket.st_size));
	memset(buffer, '\0', bucket.st_size); // Clear out the buffer array
        bytes = read(text_file, buffer, bucket.st_size);	
        if(bytes<=0){
                printf("ERROR READING FILE\n");
		exit(6);
        }
	close(text_file);
	text_file = open(name2, O_RDONLY);
	stat(name2, &bucket);
	buffer2 = malloc((sizeof(char))*(bucket.st_size));
	memset(buffer2, '\0', bucket.st_size); // Clear out the buffer array
        bytes = read(text_file, buffer2, bucket.st_size);	
        if(bytes<=0){
                printf("ERROR READING FILE\n");
		exit(6);
        }
	close(text_file);
	if(strcmp(buffer, buffer2) != 0){
	//	printf("not equal freeing data\n");
		free(buffer);
		free(buffer2);
		return 1;
	}
	else{
	//	printf("equal freeing data\n");
		free(buffer);
		free(buffer2);
		return 0;
	}
}

int main(int argc, char** argv){

        return compare();
}

