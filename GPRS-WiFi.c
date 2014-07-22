#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <fcntl.h>

#include <sys/signal.h>

#include <errno.h>

#include <termios.h>



//define interfacing details for first


#define firstBAUD B9600

#define firstDEVICE "/dev/ttyS1"



#define TRUE 1

#define FALSE 0



int fd2;

int i;

unsigned char ch1[]={0,0,0,0,0};

unsigned char a[100];


void sig_IO (int status)

{

	printf("Here\n");

	read(fd2, &ch1, 3);

	printf("ch1 :%d\n", ch1[0]);

	printf("ch1 : %d\n", ch1[1]);

	ch1[2]=0x00;

	i = atoi(ch1);

	printf("i = %d\n", i);

	sprintf(a, ". dialscript4.sh  %d", i);

	system(a);

	tcflush(fd2, TCIFLUSH);

	ch1[0]=0x00;

	ch1[1]=0x00;

}




void initfirstSerial()

{

	
	struct termios newtio2;
	struct sigaction saio2;
           /* definition of signal action */

        
	/* open the device to be non-blocking (read will return immediatly) */

	fd2 = open(firstDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if (fd2 <0) 

    {

		perror(firstDEVICE); 

		exit(-1); 

    }
       

	/* install the signal handler before making the device asynchronous */
	
saio2.sa_handler = sig_IO;

saio2.sa_flags = 0;
	
saio2.sa_restorer = NULL;
	
sigaction(SIGIO,&saio2,NULL);
          

/* allow the process to receive SIGIO */
	
fcntl(fd2, F_SETOWN, getpid());
	
/* Make the file descriptor asynchronous (the manual page says only O_APPEND and O_NONBLOCK, will work with F_SETFL...) */

	fcntl(fd2, F_SETFL, FASYNC);
 
       
	newtio2.c_cflag = firstBAUD | CS8 | CLOCAL | CREAD;

	newtio2.c_iflag = IGNPAR | ICRNL;

	newtio2.c_oflag = 0;

	newtio2.c_lflag = ICANON;

	newtio2.c_cc[VMIN]=1;

	newtio2.c_cc[VTIME]=0;
	
tcflush(fd2, TCIFLUSH);
	
tcsetattr(fd2,TCSANOW,&newtio2);     

}


//main

int main(void)

{
	
//init the first serial

	system(". dialscript3.sh");
	

	initfirstSerial();


	//init the second serial
//	
initsecondSerial();


	//make the program run forever

	while(TRUE)
	
	{


	}
	
return 0;

}
