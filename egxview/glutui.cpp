/*
 * glutui.cpp
 *
 *  Created on: 25/10/2012
 *      Author: neonman
 */

#include "glutui.h"

namespace ui {
namespace glutui{

void onDisplay(){
	/* Clear the background as black */
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Display the result */
	glutSwapBuffers();
}

void onLoad() {
	//Setup OpenGL & Glut
	int nada = 0;
	glutInit(&nada, NULL);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("My First Triangle");

	/* Extension wrangler initialising */
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		std::cerr<< "Error: "<<glewGetErrorString(glew_status)<<std::endl;
		throw new std::string("GLEW failed to load");
	}
	if (!GLEW_VERSION_2_1){
		std::cerr<< "Warning: OpenGL 2.1 not available (try Mesa libgl)"<<std::endl;
		glutDisplayFunc(onDisplay);
	}
	else{
		glutDisplayFunc(onDisplay);
	}
    glutMainLoop();
}

} /* namespace glutui*/
} /* namespace ui */
