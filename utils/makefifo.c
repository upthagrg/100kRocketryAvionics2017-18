/*********************************************
*Title: makefifo.c
*Author Glenn Upthagrove
*Date: 6/10/2018
*Description: Makes a FIFO in the filesystem
*under the name given to it from command line.
*********************************************/
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

int main(int argc, char** argv){
        int ret = 0;
        if(argc<2){
                printf("ERROR, NO FIFO NAME GIVEN\n");
                exit(1);
        }
        ret = mkfifo(argv[1], 0775);
        return ret;
}

