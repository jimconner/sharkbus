      #include <termios.h>
      #include <stdio.h>
      #include <unistd.h>
      #include <fcntl.h>
      #include <sys/signal.h>
      #include <sys/types.h>
      #include <sys/time.h>
      #include <stdlib.h>
      #include <string.h>
      #include <sys/socket.h>
      #include <arpa/inet.h>
      #include <netinet/in.h>
      
      // Serial Port Settings  
      #define SHARKBAUDRATE B38400
      #define SHARKDEVICE "/dev/ttyS1"
      #define USONICBAUDRATE B9600
      #define USONICDEVICE "/dev/ttyS2"
      #define _POSIX_SOURCE 1 /* POSIX compliant source */
      #define FALSE 0
      #define TRUE 1

      // TCP Settings
      #define MAXPENDING 5 /* Max connection requests */
      #define BUFFSIZE 32

      int handle_incoming (unsigned char buf[255], int len, unsigned char *sharkmsg, int *sharkmsg_len);
      void handle_sharkbus (unsigned char smbuf[255], int length);
      void sendmessage (unsigned char msgtype, unsigned char outbuf[32], unsigned char length);
      void Die(char *mess) { perror(mess); exit(1); }
      volatile int STOP=FALSE; 
  
      //void signal_handler_IO (int status);   /* definition of signal handler */
      int sharkfd, sharkres;
      int usonicfd, usonicres;

      void main()
      {
	fd_set readfs; /*file descriptor set */
	int maxfd; /* maximum file descriptor used */

        struct termios sharkoldtio,sharknewtio;
        struct termios usonicoldtio,usonicnewtio;
        //struct sigaction saio;           /* definition of signal action */
        unsigned char buf[255];
        unsigned char outbuf[255];

	// TCP Socket Config
	int serversock, clientsock;
	struct sockaddr_in sharkserver, sharkclient;
        
        /* open the shark device to be blocking */
        sharkfd = open(SHARKDEVICE, O_RDWR | O_NOCTTY);
        if (sharkfd <0) {perror(SHARKDEVICE); exit(-1); }
       
    #if 0 
        /* install the signal handler before making the device asynchronous */
        saio.sa_handler = signal_handler_IO;
        //saio.sa_mask = 0; //original doesn't compile
	sigemptyset(&saio.sa_mask);   //saio.sa_mask = 0;

        saio.sa_flags = 0;
        saio.sa_restorer = NULL;
        sigaction(SIGIO,&saio,NULL);
    #endif      

        /* allow the process to receive SIGIO */
        fcntl(sharkfd, F_SETOWN, getpid());
        /* Make the file descriptor asynchronous (the manual page says only 
           O_APPEND and O_NONBLOCK, will work with F_SETFL...) */
        //fcntl(sharkfd, F_SETFL, FASYNC);
        
        tcgetattr(sharkfd,&sharkoldtio); /* save current port settings */
        /* set new port settings for non-canonical input processing */
        sharknewtio.c_cflag = SHARKBAUDRATE | CS8 | CLOCAL | CREAD | CSTOPB;
        sharknewtio.c_iflag = IGNPAR | ICRNL;
        sharknewtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); 
        //sharknewtio.c_lflag = 0;
        sharknewtio.c_oflag = 0;
        sharknewtio.c_cc[VMIN]=1;
        sharknewtio.c_cc[VTIME]=0;
        tcflush(sharkfd, TCIFLUSH);
        tcsetattr(sharkfd,TCSANOW,&sharknewtio);


        /* open the ultrasonics device to be blocking */
        usonicfd = open(USONICDEVICE, O_RDWR | O_NOCTTY );
        if (usonicfd <0) {perror(USONICDEVICE); exit(-1); }

#if 0
        /* install the signal handler before making the device asynchronous */
        saio.sa_handler = signal_handler_IO;
        //saio.sa_mask = 0; //original doesn't compile
        sigemptyset(&saio.sa_mask);   //saio.sa_mask = 0;
        //memset(&saio.sa_mask, 0, sizeof(sigset_t)); // compiles ;)
        saio.sa_flags = 0;
        saio.sa_restorer = NULL;
        sigaction(SIGIO,&saio,NULL);
#endif

        /* allow the process to receive SIGIO */
        fcntl(usonicfd, F_SETOWN, getpid());
        /* Make the file descriptor asynchronous (the manual page says only
           O_APPEND and O_NONBLOCK, will work with F_SETFL...) */
        //fcntl(usonicfd, F_SETFL, FASYNC);

        tcgetattr(usonicfd,&usonicoldtio); /* save current port settings */
        /* set new port settings for canonical input processing */
        usonicnewtio.c_cflag = USONICBAUDRATE | CS8 | CLOCAL | CREAD | CSTOPB;
        usonicnewtio.c_iflag = IGNPAR | ICRNL;
        usonicnewtio.c_oflag = 0;
        usonicnewtio.c_lflag = 0;
        usonicnewtio.c_cc[VMIN]=0;
        usonicnewtio.c_cc[VTIME]=0;
        tcflush(usonicfd, TCIFLUSH);
        tcsetattr(usonicfd,TCSANOW,&usonicnewtio);

       
	/* Create TCP socket */
	if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
	  Die("Failed to create socket");
	}
	/* Construct the server sockaddr_in structure */
	memset(&sharkserver, 0, sizeof(sharkserver));       /* Clear struct */
        sharkserver.sin_family = AF_INET;                  /* Internet/IP */
        sharkserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
        sharkserver.sin_port = htons(64737);       /* server port */
	/* Bind the server socket */
	if (bind(serversock, (struct sockaddr *) &sharkserver,
				sizeof(sharkserver)) < 0) {
		Die("Failed to bind the server socket");
	}
          /* Listen on the server socket */
          if (listen(serversock, MAXPENDING) < 0) {
            Die("Failed to listen on server socket");
          }

  
        /* loop while waiting for input. normally we would do something
           useful here */ 
      
        unsigned char sharkmsg[64];
        int sharkmsg_len = 0;

        while (1) {
	    FD_ZERO (&readfs);
	    FD_SET(sharkfd, &readfs); /* set testing for ttyS port */
	    FD_SET(usonicfd, &readfs); /* set testing for ttyS port */
	    FD_SET(serversock, &readfs); /* set testing for Socket */
	    /* block until input becomes available */
	    select(maxfd, &readfs, NULL, NULL, NULL);
	    if (FD_ISSET(sharkfd, &readfs)) /* Input from ttyS available */
		    {
		    sharkres = read(sharkfd,buf,255);
      		    buf[sharkres]=0;
	    		if (sharkres>0) {
			//printf("Res: %i\n", sharkres);
	    		if(handle_incoming(buf, sharkres, sharkmsg, &sharkmsg_len)) {
	 	 		handle_sharkbus(sharkmsg, sharkmsg_len);
				sharkmsg_len = 0;
			} 
		    }
	    }
	    if (FD_ISSET(usonicfd, &readfs)) /* Input from ttyS available */
		    {
		    usonicres = read(usonicfd,buf,255);
      		    buf[usonicres]=0;
	    		if (usonicres>0) {
			printf("Res: %i\n", usonicres);
	    		//if(handle_incoming(buf, usonicres, usonicmsg, &usonicmsg_len)) {
	 	 	//	handle_usonicbus(usonicmsg, usonicmsg_len);
			//	usonicmsg_len = 0;
			//} 
		    }
	    }
	    if (FD_ISSET(serversock, &readfs)) /* Input from server socket available? */
		    {
	    		/* service TCP socket */
	    		unsigned int clientlen = sizeof(sharkclient);
	    		/* Wait for client connection */
	    		fprintf(stdout, "Waiting for connection");
	    		if ((clientsock = accept(serversock, (struct sockaddr *) &sharkclient,&clientlen)) < 0) {
					Die("Failed to accept client connection");
	    		}
	    		fprintf(stdout, "Client connected: %s\n",inet_ntoa(sharkclient.sin_addr));
	    		//HandleClient(clientsock);
	    		//FD_SET(clientsock,&readfs);
	    }
	
        }
        /* restore old port settings */
        tcsetattr(sharkfd,TCSANOW,&sharkoldtio);
        tcsetattr(usonicfd,TCSANOW,&usonicoldtio);
      }
     
#if 0   
      /***************************************************************************
      * signal handler. sets wait_flag to FALSE, to indicate above loop that     *
      * characters have been received.                                           *
      ***************************************************************************/
      void signal_handler_IO (int status)
      {
        //printf("received SIGIO signal.\n");
        wait_flag = FALSE;

      }
#endif
 
      /*******************************************************************
      * Process the incoming serial bytes from the buffer into Sharkbus  * 
      * messages - Each is terminated with 0x0f - When we've got a valid *
      * SharkBus message, we call the SharkBus handler. 	         *
      *******************************************************************/
      int handle_incoming (unsigned char buf[255], int len, unsigned char *sharkmsg, int *sharkmsg_len)
      {
  	int i;
	for(i = 0; i < len; i++) {
		*(sharkmsg + *sharkmsg_len) = buf[i];
		(*sharkmsg_len)++;
		if(buf[i] == 0x0f)
			return 1;
	}
	return 0;
      }

      /*******************************************************************
      * Process the incoming serial bytes from the buffer into Sharkbus  * 
      * messages - Each is terminated with 0x0f - When we've got a valid *
      * SharkBus message, we call the SharkBus handler. 	         *
      *******************************************************************/
      void handle_sharkbus (unsigned char smbuf[255], int length)
      {
	int msgtype;
        unsigned char outbuf[255]="";
	//int n;
	//printf("Len %i | ", length);
	//for(n=0; n < length ; n++)
	//	printf("%2.2x ",smbuf[n]);
	//printf("\n");
	msgtype = (smbuf[0] & 0x0f);	
	//if (msgtype==0) { printf("SR General Info\n"); }
	//if (msgtype==1) { printf("SPM General Info\n"); }
	//if (msgtype==2) { printf("SR HHP Data\n"); }
	//if (msgtype==3) { printf("SPM HHP Data\n"); }
	//if (msgtype==4) { printf("SR Power Up Info\n"); }
	//if (msgtype==5) { printf("SPM Power Up Info\n"); }
	//if (msgtype==6) { printf("Joystick Calibration\n"); }
	//if (msgtype==7) { printf("Factory Test\n"); }
	//if (msgtype==8) { printf("SACU General Information\n"); }
	//if (msgtype==9) { printf("SACU Power Up Information\n"); }
	//if (msgtype==10) { printf("SPM Programmable Settings\n"); }
	switch (msgtype)
	{
	 case 0:
		//printf("SR General Info\n");
		break;
	 case 1:
		//printf("SPM General Info\n");
		//printf("Sending SACU General Info - ");
          	outbuf[0] = 0xc0;
          	outbuf[1] = 0xc0;
          	outbuf[2] = 0xFF;
          	outbuf[3] = 0xC0;
          	outbuf[4] = 0x84;
          	outbuf[5] = 0x80;
		sendmessage(8, outbuf, 6);
		break;
	 case 4:
		printf("SR Power Up Info\n");
		break;
	 case 5:
		printf("SPM Power Up Info\n");
		printf("Sending SACU Power Up Info - ");
          	outbuf[0] = 0x8a;
          	outbuf[1] = 0x89;
          	outbuf[2] = 0xde;
          	outbuf[3] = 0xad;
          	outbuf[4] = 0xc5;
          	outbuf[5] = 0xa5;
          	outbuf[6] = 0x82;
		sendmessage(9, outbuf, 7);
		break;
	 default:
		printf("Huh!\n");
		break;
	}	

      }

      void sendmessage (unsigned char msgtype, unsigned char outbuf[32], unsigned char length)
      {
	int n, wres;
	unsigned char firstbyte, parity=0, sendbuf[32]="";
	firstbyte=msgtype+((length-1)<<4);

	sendbuf[0]=firstbyte;
	//printf("FB: %x\n", firstbyte);
	parity=firstbyte&127;
	for(n=0; n < length; n++) {
		parity+=(outbuf[n]&127);
		sendbuf[n+1]=outbuf[n];
		//printf("%x ", outbuf[n]);
	   }
	parity = (255-(parity & 127));
	//parity = 0xe4;
	sendbuf[n+1]=parity;
	sendbuf[n+2]=0x0f;
	//for(n=0; n < length+3; n++) 
	//	printf("%2.2x ", sendbuf[n]);
	//printf("\n");	
	wres=write(sharkfd, &sendbuf, length+3);
	//printf("Write result: %i\n", wres);
      } 

 void HandleClient(int sock) {
            char buffer[BUFFSIZE];
            int received = -1;
	    	fprintf(stdout, "In HandleClient\n");
            /* Receive message */
            if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
              Die("Failed to receive initial bytes from client");
            }
            /* Send bytes and check for more incoming data in loop */
            while (received > 0) {
	    	fprintf(stdout, "Received data\n");
              /* Send back received data */
              if (send(sock, buffer, received, 0) != received) {
                Die("Failed to send bytes to client");
              }
              /* Check for more data */
	      fprintf(stdout, "Check for more data\n");
              if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
                Die("Failed to receive additional bytes from client");
              }
            }
            //close(sock);
          }

