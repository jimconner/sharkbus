#!/usr/bin/python

import os, sys, string, time, datetime, gmpy, uspp, pygame
ser = uspp.SerialPort("/dev/ttyS2", 1000, 9600)

while True:
        ser.write(chr(0x08)+chr(0x54))
        time.sleep(0.1)
        if ser.inWaiting() > 0 :
                data = ser.read(ser.inWaiting())
                print ord(data[0]), ord(data[1])
