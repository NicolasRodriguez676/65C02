#!/bin/bash
ca65 --cpu 65c02 complex/src/rom/complex_bl.s  -I /home/nicky/Documents/6502/inc -I /home/nicky/Documents/6502/complex/src/pgm
ld65 complex/src/rom/complex_bl.o -C complex/cfg/compustart_complex.cfg