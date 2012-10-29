/*
 * capp.cpp
 *
 *  Created on: 25/10/2012
 *      Author: neonman
 */

#include "capp.h"
#include "ui.h"
#include "glutui.h"
#include "sdlui.h"

using namespace std;

#include "egx/egx.h"


//Argument 1 -> input file
int main(int argc, char* argv[]){
	//Load some test data
	ui::UiState* s = ui::UiState::getInstance();

	const char default_file[] =  "/home/neonman/egx-cnc/sample/egx/ASD.egx";
	std::ifstream* f = new std::ifstream(default_file);
	if ( ! f->fail()){
		egx::Layer* l = egx::loadLayer_egx(f);
		s->layers.push_back(*l);
		delete(l);
    	delete(f);
	}

	if (ui::glutui::onLoad(argc, argv) < 0){
	    ui::sdlui::CApp theApp;
	    return theApp.OnExecute();
	}
	else
		return 1;
}
