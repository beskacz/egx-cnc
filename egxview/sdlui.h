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
#include "ui.h"

namespace ui{
namespace sdlui{

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
        void OnEvent(SDL_Event* Event);
        void OnLoop();
        void OnRender();
        void OnCleanup();
};

} /* sdlui namespace */
} /* UI namespace */

#endif /* SDLUI_H_ */
