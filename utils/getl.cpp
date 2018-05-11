/**********************************
*Title: getl.cpp
*Author: Glenn Upthagrove
*Date: 6/10/2018
*Decscription: Gets one line at a 
*time from a file.
**********************************/
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
int main(int argc, char** argv){
	string res;
	ifstream file(argv[1]);
	while(getline(file, res)){
		cout<<"Line: " << res << endl;
	}
	return 0;
}
