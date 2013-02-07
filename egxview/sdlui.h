/*
 * sdlui.h
 *
 *  Created on: 29/10/2012
 *      Author: neonman
 */

#ifndef SDLUI_H_
#define SDLUI_H_

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <stdlib.h>
#include <boost/signals2/mutex.hpp>
#include "ui.h"
#include "egx/egx_loaders.h"
#include "egx/egx.h"

namespace ui{
namespace sdlui{

class SdlUiState {
public:
	//Attrib
	double scale_x;
	double scale_y;
	double offset_x;
	double offset_y;
	bool   redraw;
	boost::signals2::mutex ss_mutex;

	//Method
	static SdlUiState* getInstance();
protected:
	SdlUiState();
private:
	static SdlUiState* instance;
};

class CApp {
    private:
        bool    Running;
        SDL_Surface*    Surf_Display;
        unsigned long ticks;
    public:
        CApp();
        int OnExecute();
    public:
        bool OnInit();
        void OnEvent(SDL_Event* event);
        void OnLoop();
        void OnRender();
        void OnCleanup();
};

} /* sdlui namespace */
} /* UI namespace */

#endif /* SDLUI_H_ */
