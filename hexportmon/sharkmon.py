#!/usr/bin/python

# SharkBus protocol decoder/analyser
# Based on the SharkBus RS485 Communication Protocol from Dynamic Controls
# For protocol information contact dynamiccontrols.com
# Special thanks to Warren Pettigrew for his assistance. 
# Author: James Conner
# Date: 27th September 2010


import os, sys, string, time, datetime, gmpy, uspp, pygame

class SharkMonitor:
	def __init__(self):
		self.ser = uspp.SerialPort("/dev/ttyS1", 1000, 38400)

	# Read bytes until we get the End-of-Transmission byte (0x0F)
	def ReceiveMessage(self):
		messagebytes = ""
		lasthexvalue = "00"
		while lasthexvalue != "0F":
			if self.ser.inWaiting():
				data = self.ser.read(1)
			#if data:
				hexvalue = "%02X"%ord(data)
				#if lasthexvalue == "0F" : # 0x0F is the End of Transmission byte.
				#	print
				#	print datetime.datetime.now(),
				#print hexvalue,
				messagebytes = messagebytes + data
				lasthexvalue = hexvalue
		return(messagebytes)

	# Send a message, terminated with parity byte and the End-of-Transmission byte (0x0F)
	def SendMessage(self, message_type, message):
		message_length = len(message)-1 # The start byte contains the message length (00 = 1byte) and message_type
		start_byte = chr((message_length << 4)+message_type)
		message = start_byte + message
		parity = 0
		for data in message:
			parity=parity+(ord(data)&127)
		parity = (255-(parity & 127))
		parity_byte = chr(parity)
		message = message + parity_byte # Add the parity on to the end
		message = message + chr(15) # Add the EoT byte to the end.
		print "Sending Message at", datetime.datetime.now()
		self.ser.write(message)
		return()

	# Decode messages into human readable form.
	def DecodeMessage(self,message):
		print "Received at:", datetime.datetime.now()
		for data in message:
			hexvalue = "%02X"%ord(data)
			print hexvalue,
		print
	
		if ord(message[0]) == 0:
			print "Framing/Power-on startup message - Chomp"
			message = message[1:]
		# The start of message bytes all have bit 7 clear - Every other message byte has bit 7 set.
		if  ord(message[0]) > 127 or ord(message[0]) == 15:
			print "Not at message start"
			return(0)
		messagetype=(ord(message[0]) & 0x0F)
		#if messagetype == 0 : # Shark Remote General Information (Joytstick Data)
		#	joy_speed = ((ord(message[1]) & 127)<<3)+((ord(message[4]) & 56)>>3)
		#	joy_dir = ((ord(message[2]) & 127)<<3)+(ord(message[4]) & 7)
		#	speed_pot = ((ord(message[3]) & 127)<<1)+((ord(message[4]) & 64)>>6)
		#	print "Y:", joy_speed, "X:", joy_dir, "Speed:", speed_pot,


		#if messagetype == 1 : # Shark Power Module General Information
		#	fuel_guage = ord(message[1]) & 31
		#	horn = ord(message[3]) & 1
		#	driving_mode = ord(message[6]) & 15
		#	speedo = ord(message[7]) & 31
		#	print "Batt:", fuel_guage, "Horn:", horn, "Mode:", driving_mode, "Speedo:", speedo

		if messagetype == 2 : # SR HHP Data
			print "SR HHP Data"

		if messagetype == 3 : # SPM HHP Data
			print "SPM HHP Data"

		if messagetype == 4 : # SR Power Up Information
			print "SR Power Up Information"
			remote_type = ord(message[1]) & 127
			year_of_manufacture = 2000+(ord(message[2]) & 127)
			month_of_manufacture = ord(message[3]) & 15
			serial_number = ((ord(message[4]) & 127) << 14) + ((ord(message[5]) & 127) << 7) + (ord(message[6]) & 127)
			software_major = (ord(message[7]) & 56) >> 3
			software_minor = ord(message[7]) & 7
			capabilities = ord(message[8]) & 127
			print "Remote Type:", remote_type
			print "Manufactured:", month_of_manufacture, "/", year_of_manufacture
			print "Serial:", serial_number
			print "Software:"+str(software_major)+"."+str(software_minor)
			print "Capabilites:", capabilities

		if messagetype == 5 : # SPM Power Up Information
			print "SPM Power Up Information"
			capabilities = ord(message[1]) & 127
			print "Capabilities:",capabilities

		if messagetype == 6 : # Joystick Calibration
			print "Joystick Calibration (SR or ACU)"

		if messagetype == 7 : # Factory Test
			print "Factory Test"

		if messagetype == 8 : # SACU General Information
			print "SACU General Information"

		if messagetype == 9 : # SACU Power Up Information
			print "SACU Power Up Information"

		if messagetype == 10 : # SPM Programmable Settings
			print "SPM Programmable Settings"

		return(messagetype)


MyMonitor = SharkMonitor()

starttime=time.time()
while True:
	messagebytes=MyMonitor.ReceiveMessage()
	MessageType=MyMonitor.DecodeMessage(messagebytes)
	if MessageType == 5: # If we see an SPM power-up message, we should transmit our own SACU power-up message
		MyMonitor.SendMessage(9, chr(0x8a)+chr(0x89)+chr(0xDE)+chr(0xAD)+chr(0xC5)+chr(0xA5)+chr(0x82))
	if MessageType == 1: # If we see an SPM General Information Message, we should transmit our own SACU General Information message
		if time.time()-starttime < 10:
			MyMonitor.SendMessage(8, chr(0xC0)+chr(0xC0)+chr(0xFF)+chr(0xC0)+chr(0x84)+chr(0x80))
		else:
			MyMonitor.SendMessage(8, chr(0xFF)+chr(0xC0)+chr(0xFF)+chr(0xC0)+chr(0x84)+chr(0x80))

