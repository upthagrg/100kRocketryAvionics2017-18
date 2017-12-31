/****************************************************************
*Title: convert.hpp
*Author: Glenn Upthagrove
*Date: 12/30/2017
*description: A header file that givees the utilities to the 
*3D trace to convert JSON data into vertecies for OpenGL. 
****************************************************************/
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

GLuint PathList;

struct point{
	float x;
	float y;
	float z;
};

void  make_trace_list(char* filename){
	vector<struct point> vec;
	int i;
	struct point temp;
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
		strcpy(numbuff, buff+12);
		i=0;
		for(i;i<256;i++){
			if(numbuff[i]=='"'){
				break;
			}
		}
		numbuff[i] = '\0';
		temp.z=atof(numbuff);
		//longitude
		memset(buff, '\0', 256);
		memset(numbuff, '\0', 256);
		in >> buff;
		strcpy(numbuff, buff+13);
		i=0;
		for(i;i<256;i++){
			if(numbuff[i]=='"'){
				break;
			}
		}
		numbuff[i] = '\0';
		temp.x=atof(numbuff);
		//altitude
		memset(buff, '\0', 256);
		memset(numbuff, '\0', 256);
		in >> buff;
		strcpy(numbuff, buff+12);
		i=0;
		for(i;i<256;i++){
			if(numbuff[i]=='"'){
				break;
			}
		}
		numbuff[i] = '\0';
		temp.y=atof(numbuff);
		//time
		memset(buff, '\0', 256);
		in >> buff;
		vec.push_back(temp);
	}
	in.close();
	vec.pop_back();
        PathList = glGenLists( 1 );
        glNewList( PathList, GL_COMPILE );
	        glLineWidth( 1. );
	        glBegin( GL_LINE_STRIP );
	        glColor3f( 1., 0., 0. ); //red
	//      glNormal3f( 0., 0.,  1. );
	//      glVertex3f( -dx, -dy,  dz );
		i=0;
		for(i; i<vec.size(); i++){
			//cout << "x: " << vec[i].x << " y: " << vec[i].y << " z: " << vec[i].z << endl;
			glVertex3f(vec[i].x, vec[i].y, vec[i].z);
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
