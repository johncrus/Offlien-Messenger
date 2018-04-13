#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
extern int errno;
int port;
void message_request_accepted(int id1, int id2)
{
  int status;
    if(fork() == 0){ 
        char exe[300];
        bzero(exe,sizeof(exe));
        sprintf(exe,"gnome-terminal --window-with-profile=profile -e '/home/john/retele/cli3 0.0.0.0 3000 %d %d'",id1,id2);
        status = system(exe);
        exit(0);
    }
}

int main (int argc, char *argv[])
{
  int sd;			
  struct sockaddr_in server;	
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }
  port = atoi (argv[2]);
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("ERROR 1\n");
      return errno;
    }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("ERROR 2\n");
      return errno;
    }

int type=113122311;
  if (write (sd, &type, sizeof(type)) < 0)
    {
      perror ("ERROR 3\n");
      return errno;
    }
  char rec_message[1000];
  char send_message[200];
  char active=1;
  printf("\e[1;1H\e[2J");
  while(active==1)
  {
  if (read (sd, rec_message, sizeof(rec_message)) < 0)
    {
      perror ("ERROR 4\n");
      return errno;
    }
  printf("%s",rec_message);
  bzero(send_message,200);
  scanf("%s",send_message);
  if (strcmp(send_message,"quit")==0)
    active=0;
  

  if (write (sd, send_message, strlen(send_message)+1) < 0)
    {
      perror ("ERROR 5\n");
      return errno;
    }
    if (strstr(send_message,"send_message_")!=NULL)
    { 
     int data[5];
      if (read (sd, data, sizeof(data)) < 0)
         {
           perror ("ERROR 6\n");
           return errno;
         }
         if(data[0]==1)
          message_request_accepted(data[1],data[2]);
    }
    if (strstr(rec_message,"Register")==NULL || strstr(rec_message,"USERNAME: ")==NULL || strstr(send_message,"exit")!=NULL)
    printf("\e[1;1H\e[2J");
    bzero(rec_message,200);
  }
  
  close (sd);
}
