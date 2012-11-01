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
#include "egx/egx_loaders.h"
#include "ui.h"

//Do something more ... elegant. Please.

namespace ui {
namespace glutui{


int onLoad(int argc, char* argv[]);


} /* namespace glutui */
} /* namespace ui */
#endif /* GLUTUI_H_ */
