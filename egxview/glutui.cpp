/*
 * glutui.cpp
 *
 *  Created on: 25/10/2012
 *      Author: neonman
 */

#include "glutui.h"

namespace ui {
namespace glutui{

//Class: UiState
UiState* UiState::instance = 0;

UiState::UiState(){

}

UiState* UiState::getInstance(){
	if (instance == 0)
		instance = new UiState();
	return instance;
}

//Non-class procedures
void onDisplay(){
	/* Clear the background as black */
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Draw something */


	/* Display the result */
	glutSwapBuffers();
}

void onLoad(int argc, char* argv[]) {
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
		std::cerr<< "Error: OpenGL 2.1 not available."<<std::endl
				 << "       Try 'Mesa libgl' for a software OpenGL 2.1 driver"<<std::endl
				 << "       or update your graphics adapter driver."<<std::endl;
		return;
	}
	else{
		glutDisplayFunc(onDisplay);
	}

	//Load some test data
	UiState* s = UiState::getInstance();

	const char default_file[] =  "/home/neonman/egx-cnc/sample/egx/ASD.egx";
	std::ifstream* f = new std::ifstream(default_file);
	if (f->fail())
		return;
	egx::Layer* l = egx::loadLayer_egx(f);
	//pv = l->getTracks().front().getPointArray();
	//pc = l->getTracks().front().size();
	delete(l);
	delete(f);


	//Start glut
	glutMainLoop();
}

} /* namespace glutui*/
} /* namespace ui */
