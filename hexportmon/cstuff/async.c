      #include <termios.h>
      #include <stdio.h>
      #include <unistd.h>
      #include <fcntl.h>
      #include <sys/signal.h>
      #include <sys/types.h>
      #include <stdlib.h>
      #include <string.h>
        
      #define BAUDRATE B9600
      #define MODEMDEVICE "/dev/ttyS2"
      #define _POSIX_SOURCE 1 /* POSIX compliant source */
      #define FALSE 0
      #define TRUE 1
        
      volatile int STOP=FALSE; 
        
      void signal_handler_IO (int status);   /* definition of signal handler */
      int wait_flag=TRUE;                    /* TRUE while no signal received */
        
      main()
      {
        int fd, res, sensor;
        struct termios oldtio,newtio;
        struct sigaction saio;           /* definition of signal action */
        unsigned char buf[255];
        unsigned char outbuf[255];
        
        /* open the device to be non-blocking (read will return immediatly) */
        fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (fd <0) {perror(MODEMDEVICE); exit(-1); }
        
        /* install the signal handler before making the device asynchronous */
        saio.sa_handler = signal_handler_IO;
        //saio.sa_mask = 0; //original doesn't compile
	sigemptyset(&saio.sa_mask);   //saio.sa_mask = 0;
	//memset(&saio.sa_mask, 0, sizeof(sigset_t)); // compiles ;)

        saio.sa_flags = 0;
        saio.sa_restorer = NULL;
        sigaction(SIGIO,&saio,NULL);
          
        /* allow the process to receive SIGIO */
        fcntl(fd, F_SETOWN, getpid());
        /* Make the file descriptor asynchronous (the manual page says only 
           O_APPEND and O_NONBLOCK, will work with F_SETFL...) */
        fcntl(fd, F_SETFL, FASYNC);
        
        tcgetattr(fd,&oldtio); /* save current port settings */
        /* set new port settings for canonical input processing */
        newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD | CSTOPB;
        newtio.c_iflag = IGNPAR | ICRNL;
        newtio.c_oflag = 0;
        newtio.c_lflag = 0;
        newtio.c_cc[VMIN]=0;
        newtio.c_cc[VTIME]=0;
        tcflush(fd, TCIFLUSH);
        tcsetattr(fd,TCSANOW,&newtio);
         
        /* loop while waiting for input. normally we would do something
           useful here */ 
	sensor=1;
        while (STOP==FALSE) {
          //if (wait_flag==TRUE) { 
	  if (sensor==6) { sensor=1;}
          usleep(100000);
          outbuf[0] = sensor;
          outbuf[1] = 0x54;
	  res=write(fd, outbuf, strlen(outbuf));
          usleep(100000);
          printf("%i: ", sensor);
	  sensor=sensor+1;
          //printf(".\n");usleep(100000);
	  //}
          /* after receiving SIGIO, wait_flag = FALSE, input is available
             and can be read */
          if (wait_flag==FALSE) { 
            res = read(fd,buf,255);
            buf[res]=0;
	    printf("%i ", buf[0]);
	    printf("%i | ", buf[1]);
	    if (sensor>5) { printf("\n"); }
            //printf(":%s:%d\n", buf, res);
            //if (res==1) STOP=TRUE; /* stop loop if only a CR was input */
            wait_flag = TRUE;      /* wait for new input */
          }
        }
        /* restore old port settings */
        tcsetattr(fd,TCSANOW,&oldtio);
      }
        
      /***************************************************************************
      * signal handler. sets wait_flag to FALSE, to indicate above loop that     *
      * characters have been received.                                           *
      ***************************************************************************/
        
      void signal_handler_IO (int status)
      {
        //printf("received SIGIO signal.\n");
        wait_flag = FALSE;

      }
   
