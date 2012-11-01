/*
 * egx_loaders.h
 *
 *  Created on: 01/11/2012
 *      Author: neonman
 */

#ifndef EGX_LOADERS_H_
#define EGX_LOADERS_H_

#include "egx.h"
#include <istream>

namespace egx{

///Loads an egx file and returns a Layer object
Layer* loadLayer_egx(std::istream* f);

///Loads a gcode file and returns a layer object
Layer* loadLayer_gcode(std::istream* f);

} /* namespace egx */


#endif /* EGX_LOADERS_H_ */
