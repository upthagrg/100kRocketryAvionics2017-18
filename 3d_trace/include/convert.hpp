/****************************************************************
*Title: convert.hpp
*Author: Glenn Upthagrove
*Date: 01/20/2017
*description: A header file that givees the utilities to the 
*3D trace to convert JSON data into vertecies for OpenGL. 
****************************************************************/
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "./fpoint.h"
#include "../../conversion/telemetry.h"
#include "../../conversion/conversion.h"

using namespace std;

int XSCALE, YSCALE, ZSCALE;
float slat;
float slon;
float apx;
float apz;
float apy;
float apalt=0;
char apaltbuff[256]; 
bool cdebug;

GLuint PathList;
vector<struct fpoint>  make_trace_list(char* filename){
	vector<struct fpoint> vec;
	char* str;
	int i;
	int inited = 0;
	int ref = 0;
	struct fpoint temp;
	string buff;
	char numbuff[256];
	struct telem_data line;
	ifstream in;
	memset(apaltbuff, '\0', 256);
	in.open(filename);
	if(!in.is_open()){
		cout << "ERROR OPENING: " << filename << endl;
		exit(7);
	}
	str = new char[256];
	while(1){
		if(in.eof()){
			break;
		}
		//velocity
		memset(str, '\0', 256);
		getline(in, buff);
		strcpy(str, buff.c_str());
		if(strcmp(str, "**&&") == 0){
			break;
		}
		line = structure(&str);
		cout <<"getting z"<<endl;
		temp.z=((slat - line.lat) * 69.0); //HERE
		cout <<"got z: "<< temp.z<<endl;
		//longitude
		cout <<"getting x"<<endl;
		temp.x=(-(slon - line.lon) * (cos(slat * (M_PI/180.0)) * 69.172)); //HERE
		cout <<"got x: "<< temp.x<<endl;
		//altitude
		cout <<"getting y"<<endl;
		temp.y=(line.alt/5280); //conver to miles
		if(temp.y > apalt){
			apalt = temp.y;
			memset(apaltbuff, '\0', 256);
			sprintf(apaltbuff, "%f", line.alt);
		}
		cout <<"got y: "<< temp.y<<endl;
		vec.push_back(temp);
	}
	in.close();
	vec.pop_back();
	apalt = 0;
        PathList = glGenLists( 1 );
        glNewList( PathList, GL_COMPILE );
	        glLineWidth( 5. );
	        glBegin( GL_LINE_STRIP );
	        glColor3f( 1., 0., 0. ); //red
		i=0;
		for(i; i<vec.size(); i++){
			if(cdebug){
				cout << "x: " << vec[i].x << " y: " << vec[i].y << " z: " << vec[i].z << endl;
			}
			glVertex3f(vec[i].x, vec[i].y, vec[i].z);//SCALE
			if(vec[i].y > apalt){
				apalt = vec[i].y;
				ref = i;
			}
		}
		apx = vec[ref].x;
		apz = vec[ref].z;
		apy = vec[ref].y;
		if(cdebug){
			cout << "apx: " << apx << " apz: " << apz << " apy: " << apy << endl;
		}
		glEnd();
	glEndList();
	if((vec[0].x != 0.) || (vec[0].y !=0.) || (vec[0].z != 0.)){
		exit(1);
	}
	cout <<"finished conversion" <<endl;
	return vec;
}
