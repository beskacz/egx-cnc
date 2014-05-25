# EGX-CNC #

EGX-CNC is a g-code to cammgl recompiler, originally targetted at PCB milling projects on the Roland EGX-30 desktop engraver.

### What is EGX-CNC compsed of? ###

* egxmake

A g-code to cammgl recompiler. Provides g-code capabilities to cammgl mills.

* egxdrill

Creates g-code drill plots grom gerber drill files.

* egxinfo

Shows information from cammgl (.egx) files.

* egxmill

Sends a milling job to a parallel-port connected mill (Roland EGX-30)

* egxpack

Conveniently archives multiple cammgl (.egx) files

### System requierements ###

* A recent linux distribution
* Python 3

Gerber support also requires:

* pcb2gcode

Milling on cammgl devices also requires:

* A configured mill using lp/lpr or
* A parallel port connected mill
