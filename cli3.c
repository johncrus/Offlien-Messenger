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

int main (int argc, char *argv[])
{
  int sd;			
  struct sockaddr_in server;
  int nr=0;
  char buf[10];
  if (argc != 5)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  port = atoi (argv[2]);
  int from=atoi(argv[3]);
  int to=atoi(argv[4]);
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
char  bufff[300];
char printu[2000];



if (write (sd,&from,sizeof(from)) <= 0)
    {
      perror ("ERROR 3\n");
      return errno;
    }
if (write (sd,&to,sizeof(to)) <= 0)
    {
      perror ("ERROR 4\n");
      return errno;
    }
if (read (sd, printu,sizeof(printu)) < 0)
    {
      perror ("ERROR 5\n");
      return errno;
    }
    printf("%s",printu);
  while(1)
  {
  	printf("send:");
  
fgets(bufff,300,stdin);
  if (write (sd,bufff,sizeof(bufff)) <= 0)
    {
      perror ("ERROR 6\n");
      return errno;
    }
    printf("\e[1;1H\e[2J");
  if (read (sd, printu,sizeof(printu)) < 0)
    {
      perror ("ERROR 7\n");
      return errno;
    }
    printf("%s",printu);
}
  close (sd);
}
