#include <iostream>
#include <vector>
GLuint BoardList;

struct sts{
	double s;
	double t;
};

// define board texture params 
//#define BOARDWIDTH 512
//#define BOARDHEIGHT 512
void tplane(int res, float size){ 
	std::vector<struct sts> inited;
	float div = res-1;
	int cnt = 0;
	// vertecies
	BoardList = glGenLists(1);
	glNewList(BoardList, GL_COMPILE);

	glBegin(GL_TRIANGLE_STRIP);

	for (int i = 0; i < res - 1; i++) { //rows
		glNormal3f(0., 1., 0.);
		glVertex3f(0.0, 0.0, -1*(0.0+i*(size*(1/div))));
		std::cout << "coord: " << 0.0 << ", " << "0.0, " << (i)*(size*(1/div)) << std::endl;		
		glTexCoord2f(0.0, i*(1/div));
		std::cout << "out1 s: " << 0.0 << std::endl;
		std::cout << "out1 t: " << ((i)*(1/div)) << std::endl;
		glNormal3f(0., 1., 0.);
		glVertex3f(0.0, 0.0,-1*(0.0+(i+1)*(size*(1/div))));
		std::cout << "coord: " << 0.0 << ", " << "0.0, " << (i+1)*(size*(1/div)) << std::endl;
		glTexCoord2f(0.0, (i+1)*(1/div));
		std::cout << "out2 s: " << 0.0 << std::endl;
		std::cout << "out2 t: " << ((i+1)*(1/div)) << std::endl;
		for (int j = 0; j < res - 1; j++) {//cols
			cnt++;
			glNormal3f(0., 1., 0.);
			glVertex3f((size*((1/div)*(j+1))), 0.0, -size*i*(1/div));
			glTexCoord2f((j+1)*(1/div), i*(1/div));
			std::cout << "coord: " << size*((1/div)*(j+1)) << ", " << "0.0, " << -size*(i)*(1/div) << std::endl;
			std::cout << "s: " << ((j+1)*(1/div)) << std::endl;
			std::cout << "t: " << ((i)*(1/div)) << std::endl;
			glNormal3f(0., 1., 0.);
			glVertex3f((size*((1/div)*(j+1))), 0.0, -size*(i+1)*(1/div));
			glTexCoord2f((j+1)*(1/div), (i+1)*(1/div));
			std::cout << "coord: " << size*((1/div)*(j+1)) << ", " << "0.0, " << -size*(i+1)*(1/div) << std::endl;
			std::cout << "s: " << ((j+1)*(1/div)) << std::endl;
			std::cout << "t: " << ((i+1)*(1/div)) << std::endl;

		}
	}
	std::cout << "cnt: " <<  cnt << std::endl;

	glEnd();

	glEndList();
}
