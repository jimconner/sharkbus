#!/usr/bin/python

import threading

def Thread1():
	print "Starting timer 1 which will start Thread2"
	t1=threading.Timer(1,Thread2)
	t1.start()

def Thread2():
	print "Starting timer 2 which will start Thread1"
	t2=threading.Timer(1,Thread1)
	t2.start()

print "Starting Thread1"
Thread1()

