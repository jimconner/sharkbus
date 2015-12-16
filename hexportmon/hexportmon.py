#!/usr/bin/python

import serial, sys, string, datetime

ser = serial.Serial()                                                 
ser.port     = "/dev/ttyS1"                                                  
ser.baudrate = "38400"                                              
ser.parity   = "N"                                        
ser.rtscts   = "off"                                        
ser.xonxoff  = "off"
ser.stopbits = 2                                      
ser.timeout  = 1     # required so that the reader thread can exit    

try:
	ser.open()
except serial.SerialException, e:
	sys.stderr.write("Could not open serial port %s: %s\n" % (ser.portstr, e))
        sys.exit(1)

lasthexvalue = "00"
while True:
	data = ser.read(1)
	if data:
		hexvalue = "%02X"%ord(data)
		if lasthexvalue == "0F" :
			print
			print datetime.datetime.now(),

		print hexvalue,
		lasthexvalue = hexvalue


