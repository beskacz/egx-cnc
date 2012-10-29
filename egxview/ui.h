/*
 * ui.h
 *
 *  Created on: 29/10/2012
 *      Author: neonman
 */

#ifndef UI_H_
#define UI_H_

#include <list>
#include "egx/egx.h"

namespace ui {

class UiState{
public:
	static UiState* getInstance();
	std::list<egx::Layer> layers;
protected:
	UiState();
private:
	static UiState* instance;
};

} /* ui namespace*/


#endif /* UI_H_ */
