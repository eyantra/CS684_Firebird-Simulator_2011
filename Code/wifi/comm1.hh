#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <cstdio> 
#include <arpa/inet.h>
#include <cstring>
#include <pthread.h> 
#include <cstdlib>
/***********************/



char *str4;
int x=1;
int my_id =1,friend_id=2;
char ms[1024];
int nbytes;
int rfd;
char* update_t = new char[30];
struct sockaddr_in server_addr;
int server_port =6000;
float rx_speed,rx_distance;
char *send_cmd(int ,int, char *);
int open_socket(int);
void parse_comms();
void *PrintHello(void *);
char test;
char *Message_Type;
int rx_id;
int win_id,time_s;
float upd_time;
/***********************/

void itoa(int n, char s[])
{ 
  short i, j, k;
  int c, sign;

  if((sign = n) < 0) n = -n;
  i = 0;
  do { 
    s[i++] = n % 10 + '0';
  } while((n/=10) > 0);

  if(sign < 0) s[i++] = '-';
  s[i] = '\0';
  k = i-1;
  for(j=0; j<k; j++) {
    c = s[j];
    s[j] = s[k];
    s[k] = c;
    k--;
  }
}

/***********************/
char *send_cmd(int from_id,int to_id, char *type)
{
 static char *msg = NULL;
  char *from = NULL, *to = NULL;
  int msglen,bytesent;
  msg = (char *) malloc(50*sizeof(int));
  from = (char *) malloc(15*sizeof(int));
  itoa(from_id, from);
 to = (char *) malloc(15*sizeof(int));

  itoa(from_id, from);
  itoa(to_id, to);

  strcpy(msg, type);
  strcat(msg, from);
  strcat(msg, "$");
  strcat(msg, to);
  strcat(msg, "!");


  printf("msg: %s\n", msg);
  msglen = strlen(msg);
  if( send(rfd,msg,msglen,0) == -1)
			{ perror("error while sending\n");
			}
			else
			{
				//printf("bytes sending is sucessfull%s\n",msg);
					return(msg);			
			}
  //  send_msg(rfd, msg, to_port);
  
}

/***********************/
void parse_comms()
{
  //if (argc < 1) {
    //printf("USAGE:commsExample my_id my_port friend_id friend_port\n");
    //exit(1);
  //}
  
  

  // init communication
  rfd = open_socket( server_port);
  //bind_socket(rfd, *my_port);
  
  //fcntl(rfd, F_SETFL, O_NONBLOCK);

  return;
}

/***********************/

int open_socket(int server_port1)
{
  int fd;
  int yes = 1;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  /* to avoid the "address already in use" error */
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(server_port1);
  memset(&(server_addr.sin_zero), '\0', 8);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
//printf("IP address: %s\n", server_addr.sin_addr);

  
  if(connect(fd , (struct sockaddr *)&server_addr, 
           sizeof(struct sockaddr)) == -1) 
	{
			perror("connect");
			exit(1);
	}
  
  
  
  return fd;
}
/************************/

void *PrintHello(void *threadid)
{
long tid;

tid = (long)threadid;
while(1)
	{	memset( &ms, 0, sizeof( ms ) );
		nbytes=recv(rfd, ms, sizeof(ms), 0);
		test =ms[0];
		printf("RECEIVED BYTES%d%s\n",nbytes,ms);
    	//	printf("RECEIVED DATA%s\n",ms);
	}

}

