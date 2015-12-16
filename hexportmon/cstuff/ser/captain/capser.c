/* capser.c
	(C) 2004-5 Captain http://www.captain.at
	
	Helper functions for "ser"
	
	Used for testing the PIC-MMC test-board
	http://www.captain.at/electronic-index.php
*/
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

int writeport(int fd, char *chars) {
	int len = strlen(chars);
	chars[len] = 0x0d; // stick a <CR> after the command
	chars[len+1] = 0x00; // terminate the string properly
	int n = write(fd, chars, strlen(chars));
	if (n < 0) {
		fputs("write failed!\n", stderr);
		return 0;
	}
	return 1;                                                                       	                                
}

int readport(int fd, char *result) {
	int iIn = read(fd, result, 254);
	result[iIn-1] = 0x00;
	if (iIn < 0) {
		if (errno == EAGAIN) {
			printf("SERIAL EAGAIN ERROR\n");
			return 0;
		} else {
			printf("SERIAL read error %d %s\n", errno, strerror(errno));
			return 0;
		}
	}                    
	return 1;
}

int getbaud(int fd) {
	struct termios termAttr;
	int inputSpeed = -1;
	speed_t baudRate;
	tcgetattr(fd, &termAttr);
	/* Get the input speed.                              */
	baudRate = cfgetispeed(&termAttr);
	switch (baudRate) {
		case B0:      inputSpeed = 0; break;
		case B50:     inputSpeed = 50; break;
		case B110:    inputSpeed = 110; break;
		case B134:    inputSpeed = 134; break;
		case B150:    inputSpeed = 150; break;
		case B200:    inputSpeed = 200; break;
		case B300:    inputSpeed = 300; break;
		case B600:    inputSpeed = 600; break;
		case B1200:   inputSpeed = 1200; break;
		case B1800:   inputSpeed = 1800; break;
		case B2400:   inputSpeed = 2400; break;
		case B4800:   inputSpeed = 4800; break;
		case B9600:   inputSpeed = 9600; break;
		case B19200:  inputSpeed = 19200; break;
		case B38400:  inputSpeed = 38400; break;
	}
	return inputSpeed;
}
