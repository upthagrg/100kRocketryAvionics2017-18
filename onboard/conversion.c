#include "conversion.h"

/***********************************************
*Title: convert
*Params: a string pointer, a telem_data*
*Description: Converts a telem_data struct into 
*a JSON packet
***********************************************/
void convert(char** buff, struct telem_data* in){
	char str1[9] = "velocity";
	char str2[9] = "latitude";
	char str3[10] = "longitude";
	char str4[9] = "altitude";
	char str5[5] = "time";
	char str6[5] = "type";
	if(buff == NULL){
		*buff = (char*)malloc(256);
	}
	memset(*buff, '\0', 256);
	sprintf(*buff,"'{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%c\"}'", str1,in->vel,str2,in->lat,str3,in->lon,str4,in->alt,str5,in->time, str6, in->type); //make JSON string
	if(debug){
		printf("%s\n", *buff);
	}
}

/***********************************************
*Title: structure
*Params: a string pointer
*Description: Converts a JSON packet into a 
*telem_data structure
***********************************************/
struct telem_data structure(char** buff){
	char* messagecpy;
	char* token;
	struct telem_data data;


	token = (char*)malloc(sizeof(char)*256);
	messagecpy = (char*)malloc(sizeof(char)*256);
	memset(token, '\0', 256);
	memset(messagecpy, '\0', 256);
	strcpy(messagecpy, *buff);
	token = strtok(messagecpy, ":");
	token = strtok(NULL, "\"");
	data.vel = atof(token);
        token = strtok(NULL, ":");
        token = strtok(NULL, "\"");
	data.lat = atof(token);
        token = strtok(NULL, ":");
        token = strtok(NULL, "\"");
	data.lon = atof(token);
        token = strtok(NULL, ":");
        token = strtok(NULL, "\"");
	data.alt = atof(token);
        token = strtok(NULL, ":");
        token = strtok(NULL, "\"");
	data.time = atof(token);
        token = strtok(NULL, ":");
        token = strtok(NULL, "\"");
	data.type = token[0];
	return data;
}
