#!/usr/bin/python

import os, sys, string, time, datetime, gmpy, uspp, pygame
ser = uspp.SerialPort("/dev/ttyS2", 1000, 9600)

ser.write(chr(0x00)+chr(0xA0)+chr(0x00)+chr(0xAA)+chr(0x00)+chr(0xA5)+chr(0x00)+chr(0x05))
