/*
 * sdlui.cpp
 *
 *  Created on: 29/10/2012
 *      Author: neonman
 */

#include "sdlui.h"

namespace ui{
namespace sdlui{

CApp::CApp() {
	this->Running = true;
	this->Surf_Display = NULL;
	this->ticks = 0;
}

int CApp::OnExecute() {
    if(OnInit() == false) {
        return -1;
    }

    SDL_Event Event;

    while(Running) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }

        OnLoop();
        OnRender();
    }

    OnCleanup();

    return 0;
}

bool CApp::OnInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    if((Surf_Display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
        return false;
    }
    return true;
}

void CApp::OnEvent(SDL_Event* Event) {
    if(Event->type == SDL_QUIT) {
        Running = false;
    }
}

void CApp::OnLoop() {
}

void CApp::OnRender() {
	//Clear screen
	memset(this->Surf_Display->pixels, 0x00, 640*480*4);

	//Draw something
	ui::UiState* s = ui::UiState::getInstance();

	//Flip buffers
	SDL_Flip(Surf_Display);
	//Save CPU cycles
	unsigned long render_time = SDL_GetTicks() - this->ticks;
	if (render_time < 33)
		SDL_Delay(33 - render_time);
	this->ticks = SDL_GetTicks();
}

void CApp::OnCleanup() {
    SDL_Quit();
}



}
}
