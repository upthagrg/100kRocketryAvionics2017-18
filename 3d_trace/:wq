/*******************************************************
*Title: trace.cpp
*Author: Glenn Upthagrove
*Date: 01/20/18
*Description: A 3D Trace for the flight path of the 
*rocket for the high altitude rocketry challenge. 
*******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> //for strcmp

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include <convert.hpp>
#include <tplane.hpp>

int main(){
	cdebug = 1;
	//vector<struct fpoint> test = make_trace_list("~/100k/100kRocketryAvionics2017-18/3d_trace/log2.txt");
	vector<struct fpoint> test = make_trace_list("./log2.txt");
	if((test[1].z - test[0].z - 69.0) > 0.001){
		exit(1);
	}
	else if((test[1].y - test[0].y -1.0) > 0.001){
		exit(1);
	}
	return 0;	
}
