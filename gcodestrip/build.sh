#/bin/bash

mkdir build
flex gcode-simple.l
gcc -o ./build/gcodestrip gcodestrip.c

./build/gcodestrip < ../sample/gcode/front.ngc > ../sample/strip/front.bare 2>/dev/null
./build/gcodestrip < ../sample/gcode/back.ngc > ../sample/strip/back.bare 2>/dev/null
