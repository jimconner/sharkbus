#!/usr/bin/python

import os, sys, string, time, datetime, gmpy, uspp, pygame
ser = uspp.SerialPort("/dev/ttyS2", 1000, 9600)

while True:
	ser.write(chr(0x01)+chr(0x54))
	time.sleep(0.08)
	if ser.inWaiting() > 0 :
		data = ser.read(ser.inWaiting())
		print "1:", ord(data[0]), ord(data[1]),
	ser.write(chr(0x02)+chr(0x54))
	time.sleep(0.08)
	if ser.inWaiting() > 0 :
		data = ser.read(ser.inWaiting())
		print "2:", ord(data[0]), ord(data[1]),
	ser.write(chr(0x03)+chr(0x54))
	time.sleep(0.08)
	if ser.inWaiting() > 0 :
		data = ser.read(ser.inWaiting())
		print "3:", ord(data[0]), ord(data[1]),
	ser.write(chr(0x04)+chr(0x54))
	time.sleep(0.08)
	if ser.inWaiting() > 0 :
		data = ser.read(ser.inWaiting())
		print "4:", ord(data[0]), ord(data[1]),
	ser.write(chr(0x05)+chr(0x54))
	time.sleep(0.08)
	if ser.inWaiting() > 0 :
		data = ser.read(ser.inWaiting())
		print "5:", ord(data[0]), ord(data[1])

