/*
 * ui.cpp
 *
 *  Created on: 29/10/2012
 *      Author: neonman
 */

#include "ui.h"

namespace ui {

//Class: UiState
UiState* UiState::instance = 0;

UiState::UiState(){

}

UiState* UiState::getInstance(){
	if (instance == 0)
		instance = new UiState();
	return instance;
}


}
