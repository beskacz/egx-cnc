/*
 * sdlui.cpp
 *
 *  Created on: 29/10/2012
 *      Author: neonman
 */

#include "sdlui.h"

namespace ui{
namespace sdlui{

//Class: UiState
SdlUiState* SdlUiState::instance = 0;

SdlUiState::SdlUiState(){
	this->offset_x = 0.0;
	this->offset_y = 0.0;
	this->scale_x  = 3.0;
	this->scale_y  = this->scale_x;
	this->redraw = true;
}

SdlUiState* SdlUiState::getInstance(){
	if (instance == 0)
		instance = new SdlUiState();
	return instance;
}


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
    ui::sdlui::SdlUiState* ss = ui::sdlui::SdlUiState::getInstance();

    while(Running) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }

        OnLoop();
        if(ss->ss_mutex.try_lock()){
        	if(ss->redraw){
        		OnRender();
        		ss->redraw=false;
        	}
        	ss->ss_mutex.unlock();
        }
    	//Save CPU cycles
    	unsigned long render_time = SDL_GetTicks() - this->ticks;
    	if (render_time < 33)
    		SDL_Delay(33 - render_time);
    	this->ticks = SDL_GetTicks();

    }
    OnCleanup();
    return 0;
}

bool CApp::OnInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    if((Surf_Display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE)) == NULL) {
        return false;
    }
    return true;
}

void CApp::OnEvent(SDL_Event* event) {
	ui::sdlui::SdlUiState* ss = ui::sdlui::SdlUiState::getInstance();
	switch(event->type){
	case SDL_QUIT:
        Running = false;
        break;
	case SDL_VIDEORESIZE: //Create a new surface on resize event
		this->Surf_Display = SDL_SetVideoMode( event->resize.w, event->resize.h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE );
		//Resizing requires a redraw
		ss->ss_mutex.lock();
		ss->redraw = true;
		ss->ss_mutex.unlock();
		//Not being able to make a surface exits
		if (this->Surf_Display == NULL)
			Running = false;
		break;
	}
}

void CApp::OnLoop() {
}

void CApp::OnRender() {
	//Clear screen
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = this->Surf_Display->w;
	r.h = this->Surf_Display->h;
	SDL_FillRect(this->Surf_Display, &r, 0xff6495ED);//Corn flower blue ;)

	//Draw something
	ui::UiState* s = ui::UiState::getInstance();
	ui::sdlui::SdlUiState* ss = ui::sdlui::SdlUiState::getInstance();

	egx::Track t = s->layers.front().getTracks().front();
	std::list<egx::Point> p = t.getPoints();
	std::list<egx::Point>::iterator iter = p.begin();
	double last_x = iter->x * ss->scale_x;
	double last_y = iter->y * ss->scale_y;
	iter++;
	while (iter != p.end()){
		lineColor(this->Surf_Display,
				(long)((iter->x) * ss->scale_x),
				this->Surf_Display->h - (long)((iter->y) * ss->scale_y),
				(long)last_x,
				this->Surf_Display->h - (long)last_y,
				0x80808080);
		last_x = iter->x * ss->scale_x;
		last_y = iter->y * ss->scale_y;
		iter++;
	}

	lineColor(this->Surf_Display, 0, 0, 100, 100, 0x80808080);

	//Flip buffers
	SDL_Flip(Surf_Display);
}

void CApp::OnCleanup() {
    SDL_Quit();
}



}
}
