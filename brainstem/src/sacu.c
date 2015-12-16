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
      #include <roboard.h>
      
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
      #define BUFFSIZE 255

      int handle_incoming (unsigned char buf[255], int len, unsigned char *sharkmsg, int *sharkmsg_len);
      void handle_sharkbus (unsigned char smbuf[255], int length);
      void sendmessage (unsigned char msgtype, unsigned char outbuf[32], unsigned char length);
      void Die(char *mess) { perror(mess); exit(1); }
      volatile int STOP=FALSE; 
  
      //void signal_handler_IO (int status);   /* definition of signal handler */
      int sharkfd, sharkres;
      int usonicfd, usonicres, tcpres;
      int listnum;
      int sensor;
      sensor=1;
      unsigned char joydata[4];

      int main()
      {
	nice(-19); /* Set this process to be top priority for the scheduler */
	/* Set the default joydata values so that the chair doesn't move */
        joydata[0]=0xc0;
        joydata[1]=0xc0;
        joydata[2]=0xff;
        joydata[3]=0xc0;
	fd_set readfs; /*file descriptor set */
	int readsocks, maxfd; /* maximum file descriptor used */

        struct termios sharkoldtio,sharknewtio;
        struct termios usonicoldtio,usonicnewtio;
	struct timeval timeout;
	struct timeval nowtv, usonictv;
	double elapsedTime;
        //struct sigaction saio;           /* definition of signal action */
        unsigned char buf[255];
        unsigned char outbuf[255];

	// TCP Socket Config
	int serversock, clientsock;
	struct sockaddr_in sharkserver, sharkclient;
	int connectlist[5]; /* Array of sockets so we know who we are talking to */
        
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
	gettimeofday(&usonictv, NULL);
	memset((char *) &connectlist, 0, sizeof(connectlist));	/* Zero off the connect list before we begin */
        while (1) {
	    timeout.tv_usec = 75000; /*ultrasonics should return data at least every 70ms, so 75ms timeout */
	    FD_ZERO (&readfs);
	    FD_SET(sharkfd, &readfs); /* set testing for SharkBus port */
	    FD_SET(usonicfd, &readfs); /* set testing for ultrasonic port */
	    FD_SET(serversock, &readfs); /* set testing for Socket */
	    for (listnum = 0; listnum < 5; listnum++) {
		if (connectlist[listnum] != 0) {
			FD_SET(connectlist[listnum],&readfs);
			//if (connectlist[listnum] > highsock)
			//	highsock = connectlist[listnum];
		}
	    }

	    /* block until input becomes available */
	    readsocks=select(maxfd, &readfs, NULL, NULL, &timeout);
	    if (FD_ISSET(sharkfd, &readfs)) /* Input from SharkBus available */
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
	    if (FD_ISSET(usonicfd, &readfs)) /* Input from ultrasonics available */
		    {
		    usonicres = read(usonicfd,buf,255);
      		    buf[usonicres]=0;
	    		if (usonicres>0) {
			//printf("Res: %i\n", usonicres);
			printf("%i ", sensor);
			printf("%i ", buf[0]);
			printf("%i | ", buf[1]);
			sensor=sensor+1;
			if (sensor>5) { printf("\n"); sensor=1; }
	  		outbuf[0] = sensor;
          		outbuf[1] = 0x54;
          		usonicres=write(usonicfd, outbuf, strlen(outbuf));
			gettimeofday(&usonictv, NULL);
		    }
	    }
	    if (FD_ISSET(serversock, &readfs)) /* New connection on server socket available? */
		    {
	    		/* service TCP socket */
	    		unsigned int clientlen = sizeof(sharkclient);
	    		/* Wait for client connection */
	    		fprintf(stdout, "Waiting for connection");
	    		if ((clientsock = accept(serversock, (struct sockaddr *) &sharkclient,&clientlen)) < 0) {
					Die("Failed to accept client connection");
	    		}
	
			for (listnum = 0; (listnum < 5) && (clientsock != -1); listnum ++)
			if (connectlist[listnum] == 0) {
				printf("\nConnection accepted:   FD=%d; Slot=%d\n", clientsock,listnum);
				connectlist[listnum] = clientsock;
				clientsock = -1;
			}
			if (clientsock != -1) {
				/* No room left in the queue! */
				printf("\nNo room left for new client.\n");
				printf("Sorry, this server is too busy.  Try again later!\r\n");
				close(clientsock);
			}

	    		fprintf(stdout, "Client connected: %s\n",inet_ntoa(sharkclient.sin_addr));
			
	    }
	    /* Run through the list of attached clientsockets to see which need servicing */
	    for (listnum = 0; listnum < 5; listnum++) {
		if (FD_ISSET(connectlist[listnum],&readfs)) {
	    		HandleClient(connectlist[listnum]);
		}
	    } /* for (all entries in queue) */

	  /* Check to see when we last sent a packet to the ultrasonics */
	  gettimeofday(&nowtv, NULL);
	  elapsedTime = (nowtv.tv_sec - usonictv.tv_sec) * 1000;
	  elapsedTime += (nowtv.tv_usec - usonictv.tv_usec) / 1000;
	  if (elapsedTime > 75) { /* If we haven't seen a message from the usonics in 75ms we send another ping */
	  	outbuf[0] = sensor;
          	outbuf[1] = 0x54;
          	usonicres=write(usonicfd, outbuf, strlen(outbuf));
		gettimeofday(&usonictv, NULL);

	  }	
        }
        /* restore old port settings */
        tcsetattr(sharkfd,TCSANOW,&sharkoldtio);
        tcsetattr(usonicfd,TCSANOW,&usonicoldtio);
	return 0;
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
          	outbuf[0] = joydata[0];
          	outbuf[1] = joydata[1];
          	outbuf[2] = 0x80;
          	outbuf[3] = joydata[3];
          	outbuf[4] = 0x84;
          	outbuf[5] = joydata[2];
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
            /* Receive message */
            if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
              Die("Failed to receive initial bytes from client");
            }
            /* Send bytes and check for more incoming data in loop */
	    	//fprintf(stdout, "Received data\n");
		if (received==5) {
			if (buffer[0]==74) {
			joydata[0]=buffer[1];
			joydata[1]=buffer[2];
			joydata[2]=buffer[3];
			joydata[3]=buffer[4];
              /* Echo back received data */
              if (send(sock, buffer, received, 0) != received) {
                Die("Failed to send bytes to client");
		}}
              }
          }

