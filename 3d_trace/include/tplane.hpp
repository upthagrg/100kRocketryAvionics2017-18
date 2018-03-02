#include <iostream>
GLuint BoardList;

// define board texture params 
//#define BOARDWIDTH 512
//#define BOARDHEIGHT 512
void tplane(int res, float size){ 
	float div = res-1;
	//int cnt = 0;
	// vertecies
	BoardList = glGenLists(1);
	glNewList(BoardList, GL_COMPILE);

	glBegin(GL_QUADS);

	for (int i = 0; i < res - 1; i++) { //rows
		//cnt++;
		for (int j = 0; j < res - 1; j++) {//cols

			glNormal3f(0., 1., 0.);

			glVertex3f((j / div)*size, 0., (-i / div)*size);
<<<<<<< HEAD
			glTexCoord2f((j / div) - 0.0001, (i / div) - 0.0001);
=======
			glTexCoord2f((j / div), (i / div));
>>>>>>> 5917ef788261dc3584a2093789f54d6ae1e51656
			if (j / div > 1.0) {
				std::cout << "S ERROR" << std::endl;
			}

<<<<<<< HEAD
			glVertex3f(((j + 1) / div)*size, 0., (-i / div)*size);
			glTexCoord2f(((j + 1) / div) - 0.0001, (i / div) - 0.0001);
=======
			glNormal3f(0., 1., 0.);
			glVertex3f(((j + 1) / div)*size, 0., (-i / div)*size);
			glTexCoord2f(((j+1 ) / div), (i / div));
>>>>>>> 5917ef788261dc3584a2093789f54d6ae1e51656
			if ((j + 1) / div > 1.0) {
				std::cout << "S ERROR" << std::endl;
			}

<<<<<<< HEAD
			glVertex3f(((j + 1) / div)*size, 0., (-(i + 1) / div)*size);
			glTexCoord2f(((j + 1) / div) - 0.0001, ((i + 1) / div) - 0.0001);
=======
			glNormal3f(0., 1., 0.);
			glVertex3f(((j + 1) / div)*size, 0., (-(i + 1) / div)*size);
			glTexCoord2f(((j+1 ) / div), ((i + 1) / div));
//			if((((j+1)/div) - 1.0) < 0.0001){
//				std::cout << "hit 1" << std::endl;
//			}
>>>>>>> 5917ef788261dc3584a2093789f54d6ae1e51656
			if ((j + 1) / div > 1.0) {
				std::cout << "S ERROR" << std::endl;
			}


<<<<<<< HEAD
			glVertex3f((j / div)*size, 0., (-(i + 1) / div)*size);
			glTexCoord2f((j / div) - 0.0001, ((i + 1) / div) - 0.0001);
=======
			glNormal3f(0., 1., 0.);
			glVertex3f((j / div)*size, 0., (-(i + 1) / div)*size);
			glTexCoord2f((j / div), ((i + 1) / div));
>>>>>>> 5917ef788261dc3584a2093789f54d6ae1e51656
			if (j / div > 1.0) {
				std::cout << "S ERROR" << std::endl;
			}

		}
	}
	//std::cout << cnt << std::endl;

	glEnd();

	glEndList();
}
