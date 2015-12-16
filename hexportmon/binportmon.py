#!/usr/bin/python

import serial, sys, string, datetime, gmpy

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
lastbutone = "00"
while True:
	data = ser.read(1)
	if data:
		hexvalue = "%02X"%ord(data)
		binvalue = gmpy.digits(ord(data),2).zfill(8) 
		if lasthexvalue == "00" and lastbutone == "78":
			print
			print datetime.datetime.now(),
		#if hexvalue == "39" and lasthexvalue == "00":
		#	print
		#	print datetime.datetime.now(),

		print binvalue,
		lastbutone = lasthexvalue
		lasthexvalue = hexvalue


