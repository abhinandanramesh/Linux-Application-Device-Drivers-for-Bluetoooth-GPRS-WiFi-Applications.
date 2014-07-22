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




int bt_init(void);



//define interfacing details for first


#define firstBAUD B9600

#define firstDEVICE "/dev/ttyS4"



#define TRUE 1

#define FALSE 0



int fd2;

int fd_bt;


unsigned char ch2[10];



void sig_IO (int status)

{

	read(fd2, &ch2, 1);

	printf("String %c \n", ch2[0]);


	switch(ch2[0])

	{

		case 'f':

				printf("String %c \n", ch2[0]);
	
			system("echo f >> /dev/ttyS1");

				printf("String %c \n", ch2[0]);

				break;
		
		case 'n':

				printf("String %c \n", ch2[0]);
		
		                system("echo n >> /dev/ttyS1");

				printf("String %c \n", ch2[0]);

				break;
		
		default:
	
				printf("String %c \n", ch2[0]);

				break;

	}

	tcflush(fd2, TCIFLUSH);

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
	

	printf("Connect BT\n");

	
	bt_init();

	
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


int bt_init(void)
{

	unsigned char ch;

	unsigned int i;

	unsigned char  * s2;
	
unsigned char s3[100];

	struct termios options;

	
	fd_bt = open("/dev/ttyS4", O_RDWR | O_NOCTTY | O_NDELAY);

	
	tcgetattr(fd_bt, &options);
	
cfsetispeed(&options, B9600);
	
cfsetospeed(&options, B9600);

	tcsetattr(fd_bt, TCSANOW, &options);

	
	while(ch != 'K' )

	{
		
read(fd_bt, &ch, 1);
	
}

	
s0:	system("echo -e \"\r\nLLL\" > /dev/ttyS4");

	
	while(ch != 'K' )

	{
	
	read(fd_bt, &ch, 1);

		if(ch == 'R')
		
	goto s0;

	}

	
	ch=0x00;

	printf("1st cmd\n");


	sleep(1);

s1:	system("echo -e \"\r\nAT+MODE=1\" > /dev/ttyS4");
	
	
while(ch != 'K' )
	
{
		
read(fd_bt, &ch, 1);

		if(ch == 'R')
	
		goto s1;
	
}
	
	
ch=0x00;

	printf("2nd cmd\n");

	sleep(1);
	

s2:	system("echo -e \"\r\nAT+BDISCMODE=3,0\" > /dev/ttyS4");
	

	while(ch != 'K' )

	{
		
read(fd_bt, &ch, 1);
		
if(ch == 'R')

			goto s2;

	}

	
	ch=0x00;

	printf("3rd cmd\n");

	sleep(1);
	
	
return 0;

}
