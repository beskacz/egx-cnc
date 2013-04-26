#ifndef __PPWRITE_H
#define __PPWRITE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ieee1284.h>

//Constants
#define PP_FREAD_BUFFER (1048576) /*A megabyte*/
#define PP_BLOCK_SIZE   16

//Globals
struct parport pp;
struct parport_list pp_lst;

//Port capabilities
uint8_t pp_has_ecp;
uint8_t pp_has_compat;
uint8_t pp_has_epp;

//Return values
#define PP_OK         0
#define PP_ERROR      1
#define PP_NOTFOUND   2
#define PP_SYS        3

//
#endif
