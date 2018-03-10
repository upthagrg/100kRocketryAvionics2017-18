/********************************************************
* Title: telemetry.h
* Author: Glenn Upthagrove 
* Date: 03/07/2018
* Description: A header file containing the definition 
* of the telem_data struct. 
********************************************************/
#ifndef __TELEMDATA
#define __TELEMDATA
struct telem_data{
	float alt;
	float vel;
	float lat;
	float lon;
	float time;
};
#endif
