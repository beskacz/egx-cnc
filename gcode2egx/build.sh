#/bin/bash

mkdir build
flex gcode-simple.l
gcc -o ./build/gcodestrip gcodestrip.c
