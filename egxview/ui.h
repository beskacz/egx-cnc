/*
 * ui.h
 *
 *  Created on: 29/10/2012
 *      Author: neonman
 */

#ifndef UI_H_
#define UI_H_

namespace ui {

class UiState{
public:
	static UiState* getInstance();
protected:
	UiState();
private:
	static UiState* instance;
};

} /* ui namespace*/


#endif /* UI_H_ */
