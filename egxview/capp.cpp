/*
 * capp.cpp
 *
 *  Created on: 25/10/2012
 *      Author: neonman
 */

#include "capp.h"
#include "glutui.h"
#include "sdlui.h"

using namespace std;

#include "egx/egx.h"


//Argument 1 -> input file
int main(int argc, char* argv[]){
	if (ui::glutui::onLoad(argc, argv) < 0){
		return(ui::sdlui::onLoad(argc, argv));
	}
	else
		return 1;
}
