#!/usr/bin/python

import serial, sys, string, datetime

ser = serial.Serial()                                                 
ser.port     = "/dev/ttyS1"                                                  
ser.baudrate = "38400"                                              
ser.parity   = "N"                                        
ser.rtscts   = "off"                                        
ser.xonxoff  = "off"                                       
ser.timeout  = 1     # required so that the reader thread can exit    

try:
	ser.open()
except serial.SerialException, e:
	sys.stderr.write("Could not open serial port %s: %s\n" % (ser.portstr, e))
        sys.exit(1)

lasthexvalue = "00"
lasttime=datetime.datetime.now()
while True:
	data = ser.read(1)
	if data:
		hexvalue = "%02X"%ord(data)
		#if hexvalue == "78" and lasthexvalue == "00":
		#	print
		#	print datetime.datetime.now(),
		#if hexvalue == "4F" and lasthexvalue == "00":
		#	print
		#	print datetime.datetime.now(),
		#if hexvalue == "39" and lasthexvalue == "00":
		#	print
		#	print datetime.datetime.now(),

		timenow=datetime.datetime.now()
		print (timenow.microsecond-lasttime.microsecond), hexvalue
		lasthexvalue = hexvalue
		lasttime = timenow


