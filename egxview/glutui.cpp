/*
 * glutui.cpp
 *
 *  Created on: 25/10/2012
 *      Author: neonman
 */

#include "glutui.h"

namespace ui {
namespace glutui{


//Non-class procedures
void onDisplay(){
	/* Clear the background as black */
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Draw something */


	/* Display the result */
	glutSwapBuffers();
}

int onLoad(int argc, char* argv[]) {
	return -1;
	//Setup OpenGL & Glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("egxView");

	/* Extension wrangler initialising */
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		std::cerr<< "Error: "<<glewGetErrorString(glew_status)<<std::endl;
		throw new std::string("GLEW failed to load");
	}
	if (!GLEW_VERSION_2_1){
		std::cerr<< "Warning: OpenGL 2.1 not available. Using software UI"<<std::endl
				 << "         Try 'Mesa libgl' for a software OpenGL 2.1 driver"<<std::endl
				 << "         or update your graphics adapter driver."<<std::endl;
		return -1;
	}
	else{
		glutDisplayFunc(onDisplay);
	}

	//Start glut
	glutMainLoop();
	return 1;
}

} /* namespace glutui*/
} /* namespace ui */
