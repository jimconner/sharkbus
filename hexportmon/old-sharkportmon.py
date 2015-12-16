#!/usr/bin/python

# SharkBus protocol decoder/analyser
# Based on the SharkBus RS485 Communication Protocol from Dynamic Controls
# For protocol information contact dynamiccontrols.com
# Special thanks to Warren Pettigrew for his assistance. 
# Author: James Conner
# Date: 27th September 2010


import serial, os, sys, string, datetime, gmpy
#from construct import *

class SharkMonitor:
	def __init__(self):
		self.ser = serial.Serial()                                                 
		self.ser.port     = "/dev/ttyS1"                                                  
		self.ser.baudrate = "38400"                                              
		self.ser.parity   = "N"                                        
		self.ser.rtscts   = "off"                                        
		self.ser.xonxoff  = "off"
		self.ser.stopbits = 2                                      
		self.ser.timeout  = 1     # required so that the reader thread can exit    
		try:
			self.ser.open()
		except serial.SerialException, e:
			sys.stderr.write("Could not open serial port %s: %s\n" % (ser.portstr, e))
		       	sys.exit(1)

	# Read bytes until we get the End-of-Transmission byte (0x0F)
	def ReceiveMessage(self):
		messagebytes = ""
		lasthexvalue = "00"
		while lasthexvalue != "0F":
			data = self.ser.read(1)
			if data:
				hexvalue = "%02X"%ord(data)
				#if lasthexvalue == "0F" : # 0x0F is the End of Transmission byte.
				#	print
				#	print datetime.datetime.now(),
				#print hexvalue,
				messagebytes = messagebytes + data
				lasthexvalue = hexvalue
		return(messagebytes)

	# Decode messages into human readable form.
	def DecodeMessage(self,message):
		#for data in message:
		#	hexvalue = "%02X"%ord(data)
		#	print hexvalue,
	
		# The start of message bytes all have bit 7 clear - Every other message byte has bit 7 set.
		if  ord(message[0]) > 127 or ord(message[0]) == 15:
			if ord(message[0]) == 0:
				print "Framing/Power-on startup message - TODO"
			print "Not at message start"
			return(0)
		messagetype=(ord(message[0]) & 0x0F)
		if messagetype == 0 : # Shark Remote General Information (Joytstick Data)
			#SRGenInfo = Struct("SR_info_message",
			#		BitStruct("packet_header",
			#			Flag("data_byte"),
			#			Bits("packet_length", 3),
			#			Bits("message_type", 4),
			#			),
			#		BitStruct("joydata_1",
			#			Flag("data_byte"),
			#			Bits("joy_speed_msb", 7),
			#			),
			#		BitStruct("joydata_2",
			#			Flag("data_byte"),
			#			Bits("joy_dir_msb", 7),
			#			),
			#		BitStruct("joydata_3",
			#			Flag("data_byte"),
			#			Bits("speed_pot_msb", 7),
			#			),
			#		BitStruct("joydata_4",
			#			Flag("data_byte"),
			#			Bits("speed_pot_lsb", 1),
			#			Bits("joy_speed_lsb", 3),
			#			Bits("joy_dir_lsb", 3),
			#			),
			#		BitStruct("error-horn",
			#			Flag("data_byte"),
			#			Flag("joystick_error"),
			#			Flag("speed_pot_error"),
			#			Flag("local_fault"),
			#			Flag("batt_charge_inhibit"),
			#			Flag("power_switch_state"),
			#			Flag("horn_swtich_state"),
			#			Flag("lock_switch_state"),
			#			),
			#		BitStruct("lights-pgm",
			#			Flag("data_byte"),
			#			Flag("hazard_light_request"),
			#			Flag("left_indicator_request"),
			#			Flag("right_indicator_request"),
			#			Flag("remote_inhibit"),
			#			Flag("hhp_comms_flow"),
			#			Flag("joystick_calibration"),
			#			Flag("sleep_confirm"),
			#			),
			#		BitStruct("headlight_actuators",
			#			Flag("data_byte"),
			#			Flag("not_used"),
			#			Flag("local_noncrit_fault"),
			#			Flag("headlight_request"),
			#			Bits("operating_mode", 4),
			#			),
			#		BitStruct("checksum",
			#			Flag("data_byte"),
			#			Bits("checksum", 7),
			#			),
			#		)
			#parsed_data = SRGenInfo.parse(message[0:9])
			#joy_speed = ((parsed_data.joydata_1.joy_speed_msb)*8)+parsed_data.joydata_4.joy_speed_lsb
			#joy_dir = ((parsed_data.joydata_2.joy_dir_msb)*8)+parsed_data.joydata_4.joy_dir_lsb
			#speed_pot = (( parsed_data.joydata_3.speed_pot_msb)*2)+parsed_data.joydata_4.speed_pot_lsb
			#print SRGenInfo.parse(message[0:9]).joydata_4
			joy_speed = ((ord(message[1]) & 127)<<3)+((ord(message[4]) & 56)>>3)
			joy_dir = ((ord(message[2]) & 127)<<3)+(ord(message[4]) & 7)
			speed_pot = ((ord(message[3]) & 127)<<1)+((ord(message[4]) & 64)>>6)
			print "Y:", joy_speed, "X:", joy_dir, "Speed:", speed_pot,


		if messagetype == 1 : # Shark Power Module General Information
			#SPMGenInfo = Struct("SPM_info_message",
			#		BitStruct("packet_header",
			#			Flag("data_byte"),
			#			Bits("packet_length", 3),
			#			Bits("message_type", 4),
			#			),
			#		BitStruct("lock_battguage",
			#			Flag("data_byte"),
			#			Flag("lock_status"),
			#			Flag("dci_slowdown"),
			#			Bits("fuel_guage", 5),
			#			),
			#		BitStruct("inhibit_state",
			#			Flag("data_byte"),
			#			Flag("drive_inhibited"),
			#			Flag("display_disable"),
			#			Flag("power_down_now"),
			#			Bits("system_state", 4),
			#			),
			#		BitStruct("lock_sleep_horn",
			#			Flag("data_byte"),
			#			Flag("lock_mode_enable"),
			#			Flag("sr_temp_drive_mod"),
			#			Flag("joystick_wake_allowed"),
			#			Flag("sleep_mode_requested"),
			#			Flag("joystick_calbiration_mode"),
			#			Flag("not_used"),
			#			Flag("sound_horn"),
			#			),
			#		BitStruct("acu_lights1",
			#			Flag("data_byte"),
			#			Flag("attendant_user"),
			#			Flag("system_is_driving"),
			#			Flag("light_flash_cadence"),
			#			Flag("hazard_light_status"),
			#			Flag("left_indicator_status"),
			#			Flag("right_indicator_status"),
			#			Flag("headlight_status"),
			#			),
			#		BitStruct("acu_lights2",
			#			Flag("data_byte"),
			#			Flag("sacu_joystick_calibration"),
			#			Flag("sacu_temp_drive_mod"),
			#			Flag("sacu_powerup_ack"),
			#			Flag("hazard_light_fault"),
			#			Flag("left_indicator_fault"),
			#			Flag("right_indicator_fault"),
			#			Flag("headlight_fault"),
			#			),
			#		BitStruct("actuator_mode",
			#			Flag("data_byte"),
			#			Flag("actuator2_fault"),
			#			Flag("actuator1_fault"),
			#			Flag("sr_requested_invalid_mode"),
			#			Bits("operating_mode", 4),
			#			),
			#		BitStruct("speedo",
			#			Flag("data_byte"),
			#			Flag("not_used"),
			#			Flag("sacu_requested_invalid_mode"),
			#			Bits("speedo_reading", 5),
			#			),
			#		BitStruct("checksum",
			#			Flag("data_byte"),
			#			Bits("checksum", 7),
			#			),
			#		)
			#print SPMGenInfo.parse(message[0:9])
			fuel_guage = ord(message[1]) & 31
			horn = ord(message[3]) & 1
			driving_mode = ord(message[6]) & 15
			speedo = ord(message[7]) & 31
			print "Batt:", fuel_guage, "Horn:", horn, "Mode:", driving_mode, "Speedo:", speedo
		return len(message)

MyMonitor = SharkMonitor()
while True:
	messagebytes=MyMonitor.ReceiveMessage()
	Length=MyMonitor.DecodeMessage(messagebytes)
	
