#!/bin/bash
ca65 -o simple/src/rom/simple_bl.o --cpu 65c02 simple/src/rom/simple_bl.s -I /home/nicky/Documents/6502/inc -I /home/nicky/Documents/6502/simple/src/pgm
ld65 -C simple/cfg/compustart.cfg simple/src/rom/simple_bl.o 
