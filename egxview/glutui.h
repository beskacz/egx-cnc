/*
 * glutui.h
 *
 *  Created on: 25/10/2012
 *      Author: neonman
 */

#ifndef GLUTUI_H_
#define GLUTUI_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include "egx/egx.h"

//Do something more ... elegant. Please.

namespace ui {
namespace glutui{


class UiState{
public:
	static UiState* getInstance();
protected:
	UiState();
private:
	static UiState* instance;
};

void onLoad(int argc, char* argv[]);


} /* namespace glutui */
} /* namespace ui */
#endif /* GLUTUI_H_ */
