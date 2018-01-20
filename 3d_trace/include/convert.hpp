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
#include <vector>
#include <cmath>
using namespace std;

float slat;
float slon;
float apx;
float apz;
float apalt=0;
bool cdebug;

GLuint PathList;

struct fpoint{
	float x;
	float y;
	float z;
};

void  make_trace_list(char* filename){
	vector<struct fpoint> vec;
	int i;
	int inited = 0;
	int ref = 0;
	//float lat;
	struct fpoint temp;
	char buff[256];
	char numbuff[256];
	ifstream in;
	in.open(filename);

	while(1){
		if(in.eof()){
			break;
		}
		//velocity
		memset(buff, '\0', 256);
		in >> buff;
		//latitude
		memset(buff, '\0', 256);
		memset(numbuff, '\0', 256);
		in >> buff;
		//cout << "lat:" <<buff << endl;
		strcpy(numbuff, buff+12);
		i=0;
		for(i;i<256;i++){
			if(numbuff[i]=='"'){
				break;
			}
		}
		numbuff[i] = '\0';
		if(inited == 0){
			slat = atof(numbuff);
		}
		temp.z=((slat - atof(numbuff)) * 69.0); //HERE
		//lat=atof(numbuff); //HERE
		//longitude
		memset(buff, '\0', 256);
		memset(numbuff, '\0', 256);
		in >> buff;
		//cout << "lon:" <<buff << endl;
		strcpy(numbuff, buff+13);
		i=0;
		for(i;i<256;i++){
			if(numbuff[i]=='"'){
				break;
			}
		}
		numbuff[i] = '\0';
		if(inited == 0){
			slon = atof(numbuff);
			inited = 1;
		}
		temp.x=((slon - atof(numbuff)) * (cos(slat) * 69.172)); //HERE
		//altitude
		memset(buff, '\0', 256);
		memset(numbuff, '\0', 256);
		in >> buff;
		//cout << "alt:" <<buff << endl;
		strcpy(numbuff, buff+12);
		i=0;
		for(i;i<256;i++){
			if(numbuff[i]=='"'){
				break;
			}
		}
		numbuff[i] = '\0';
		temp.y=(atof(numbuff)/5280); //conver to miles
		//temp.y=(atof(numbuff)); 
		//time
		memset(buff, '\0', 256);
		in >> buff;
		//add struct
		vec.push_back(temp);
	}
	in.close();
	vec.pop_back();
//	slat = vec[0].z;
//	slon = vec[0].x;
        PathList = glGenLists( 1 );
        glNewList( PathList, GL_COMPILE );
	        glLineWidth( 5. );
	        glBegin( GL_LINE_STRIP );
	        glColor3f( 1., 0., 0. ); //red
	//      glNormal3f( 0., 0.,  1. );
	//      glVertex3f( -dx, -dy,  dz );
		i=0;
		for(i; i<vec.size(); i++){
			if(cdebug){
				cout << "x: " << vec[i].x << " y: " << vec[i].y << " z: " << vec[i].z << endl;
			}
			glVertex3f(vec[i].x, vec[i].y, vec[i].z);
			if(vec[i].y > apalt){
				apalt = vec[i].y;
				ref = i;
			}
		}
		apx = vec[ref].x;
		apz = vec[ref].z;
		if(cdebug){
			cout << "apx: " << apx << " apz: " << apz << endl;
		}
		glEnd();
	glEndList();
}
/*
int main(){
	make_trace_list("log.txt");
	return 0;
}
*/
