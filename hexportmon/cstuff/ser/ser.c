/* ser.c
	(C) 2004-5 Captain http://www.captain.at
	
	Sends 3 characters (ABC) via the serial port (/dev/ttyS0) and reads
	them back if they are returned from the PIC.
	
	Used for testing the PIC-MMC test-board
	http://www.captain.at/electronic-index.php

*/
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include "captain/capser.h"

int fd;

int initport(int fd) {
	struct termios options;
	// Get the current options for the port...
	tcgetattr(fd, &options);
	// Set the baud rates to 9600...
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	// Enable the receiver and set local mode...
	options.c_cflag |= (CLOCAL | CREAD);

	options.c_cflag &= ~PARENB;
	options.c_cflag |= CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_cflag &= ~CRTSCTS;
	options.c_iflag = 0;
	
	options.c_cc[VTIME] = 0; /* inter-character timer unused */
	options.c_cc[VMIN] = 1; /* blocking read until 1 char received */

	// Set the new options for the port...
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);
	return 1;
}

int main(int argc, char **argv) {

	fd = open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		perror("open_port: Unable to open /dev/ttyS2 - ");
		return 1;
	} else {
		fcntl(fd, F_SETFL, 0);
	}
	
	printf("baud=%d\n", getbaud(fd));
	initport(fd);
	printf("baud=%d\n", getbaud(fd));

	char sCmd[254];
	sCmd[0] = 0x01;
	sCmd[1] = 0x54;

	if (!writeport(fd, sCmd)) {
		printf("write failed\n");
		close(fd);
		return 1;
	}

	printf("written:%s\n", sCmd);
	
	usleep(500000);
	//char sCmd[254];
	sCmd[0] = 0x02;
	sCmd[1] = 0x54;

	if (!writeport(fd, sCmd)) {
		printf("write failed\n");
		close(fd);
		return 1;
	}

	printf("written:%s\n", sCmd);
	
	usleep(500000);
	//char sCmd[254];
	sCmd[0] = 0x05;
	sCmd[1] = 0x54;

	if (!writeport(fd, sCmd)) {
		printf("write failed\n");
		close(fd);
		return 1;
	}

	printf("written:%s\n", sCmd);
	
	usleep(500000);
	char sResult[254];
	fcntl(fd, F_SETFL, FNDELAY); // don't block serial read

	if (!readport(fd,sResult)) {
		printf("read failed\n");
		close(fd);
		return 1;
	}
	printf("readport=%s\n", sResult);
	close(fd);
	return 0;
}
