CFLAGS=-g -Wall 
ser: captain/capser.c ser.c
	gcc -g -c -Wall captain/capser.c -o capser.o
	gcc -g -c -Wall ser.c -o ser.o
	gcc ser.o capser.o -o ser
