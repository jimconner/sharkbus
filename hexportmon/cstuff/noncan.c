      #include <sys/types.h>
      #include <sys/stat.h>
      #include <fcntl.h>
      #include <termios.h>
      #include <stdio.h>
      #include <stdlib.h>
      #include <string.h>
      #include <unistd.h>
        
      #define BAUDRATE B9600
      #define MODEMDEVICE "/dev/ttyS2"
      #define _POSIX_SOURCE 1 /* POSIX compliant source */
      #define FALSE 0
      #define TRUE 1
        
      volatile int STOP=FALSE; 
      
      main()
      {
        int fd,c, res;
        struct termios oldtio,newtio;
        unsigned char buf[255];
        
        fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY ); 
        if (fd <0) {perror(MODEMDEVICE); exit(-1); }
        
        tcgetattr(fd,&oldtio); /* save current port settings */
        
        bzero(&newtio, sizeof(newtio));
        newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD | CSTOPB;
        newtio.c_iflag = IGNPAR;
        newtio.c_oflag = 0;
        
        /* set input mode (non-canonical, no echo,...) */
        newtio.c_lflag = 0;
         
        newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
        newtio.c_cc[VMIN]     = 2;   /* blocking read until 2 chars received */
        
        tcflush(fd, TCIFLUSH);
        tcsetattr(fd,TCSANOW,&newtio);

        while (STOP==FALSE) {       /* loop for input */
       
        char sCmd[254];
        sCmd[0] = 0x05;
        sCmd[1] = 0x54;

        if (!writeport(fd, sCmd)) {
                printf("write failed\n");
                close(fd);
                return 1;
        }
	//printf("written:%s\n", sCmd);

	usleep(100000);
        
          res = read(fd,buf,255);   /* returns after 2 chars have been input */
          buf[res]=0;               /* so we can printf... */
          printf("%i ", buf[0]);
          printf("%i\n", buf[1]);
          //printf(":%s:%d\n", buf, res);
          if (buf[0]=='z') STOP=TRUE;
        }
        tcsetattr(fd,TCSANOW,&oldtio);
      }

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

