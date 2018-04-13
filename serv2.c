#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#define PORT 3000
extern int errno;

struct users{
  int id;
  char name[50];
}database[30];
void build_structure(struct users database[])
{
  char temp[100];
  FILE *fpacc;
  int i=1;
  if((fpacc = fopen("accounts.config","r"))==NULL) // deschid fisierul cu conturi
        perror("Open accounts file error");
  while(fgets(temp,sizeof(temp),fpacc)!=NULL) // cautam username-ul
        {  
          database[i].id=i;
          strcpy(database[i].name,temp);
          i=i+1;
        }
  close(fpacc);} 
 struct messages_database{
  char conv[2000];
  int message_id;
}conversations[30][30];
typedef struct thData{
  int idThread; 
  int cl; 
}thData;

void reply(const char * messages,int from, int to) //functia care se ocupa de reply
{   char mess[300];
  strcpy(mess,messages);
    int i=6;
   
    char nr[10];
    int k=0;
    while(mess[i]!='>' && mess[i]!=NULL)
    {
      nr[k]=mess[i];
      k=k+1;
      i=i+1;
    }
    int x=atoi(nr);

    char decautat[20];
    char decautat2[20];
    sprintf(decautat,"<%d>",x+1);
    sprintf(decautat2,"<%d>",x);
    char * p;
    p=strstr(conversations[from][to].conv,decautat);
    char aux[300];
    strcpy(aux,p);
    int z=strlen(conversations[from][to].conv)-strlen(p);
    strcpy(conversations[from][to].conv+z,"|_____");
     time_t rawtime;
     struct tm * timeinfo;
     time ( &rawtime );

    timeinfo = localtime ( &rawtime );
     char numele[50];
    bzero(numele,50);
    strncpy(numele,database[from].name,strlen(database[from].name)-1);
    char message_id_char[50];
    char message1[300];
    sprintf(message1, "[%d:%d:%d]:%s| ",timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,numele);
    conversations[from][to].message_id=conversations[from][to].message_id+1;
    sprintf(message_id_char,"<%d>",conversations[from][to].message_id);
    strcat(conversations[from][to].conv,message_id_char);
    strcat(conversations[from][to].conv,message1);
    strcat(conversations[from][to].conv,mess);
    strcat(conversations[from][to].conv,aux);



    strcpy(conversations[to][from].conv,conversations[from][to].conv);
    conversations[to][from].message_id=conversations[from][to].message_id;}
int n_matrix(struct users database[])
{
  int j=1;
  while(database[j].id!=0)
  {j=j+1;}
  return j-1;}
void update_messages(int q, int k,struct users database[])
{
  int i;
  int v[100];
  FILE *file1;
  file1=fopen("messages.config", "r");
 int n=n_matrix(database);
 for(i = 1; i <=n; i++)
  {

       if (!fscanf(file1, "%d", &v[i])) 
           break;
  }
  fclose(file1);

  FILE *file2;
  file2=fopen("messages.config", "w");
  int aux=0;
  while(v[q]>0)
   {  if (v[q]%10!=k)
      aux=aux*10+v[q]%10;
      v[q]=v[q]/10;


   }
  v[q]=aux;
 for(i = 1; i <=n; i++)
  {     
         fprintf(file2,"%d \n",v[i]);     
  }
  fclose(file2);}
static void *treat(void *); 
void raspunde(void *);


int main ()
{
  struct sockaddr_in server;  
  struct sockaddr_in from;  
  int nr;   
  int sd;  
  int pid;
  pthread_t th[100];    
  int i=0;
  
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("ERROR 1\n");
      return errno;
    }
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  server.sin_family = AF_INET;  
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (PORT);
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("ERROR 2\n");
      return errno;
    }

  if (listen (sd, 10) == -1)
    {
      perror ("ERROR 3\n");
      return errno;
    }
    build_structure(database);

  while (1)
    {
      int client;
      thData * td;    
      int length = sizeof (from);
      fflush (stdout);
      if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
      {
       perror ("ERROR 4\n");
       continue;
      }

    
  int idThread; 
  int cl; 

  td=(struct thData*)malloc(sizeof(struct thData)); 
  td->idThread=i++;
  td->cl=client;

  pthread_create(&th[i], NULL, &treat, td);       
        
  }   
};        
static void *treat(void * arg)
{   
    struct thData tdL; 
    tdL= *((struct thData*)arg);  
    printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
    fflush (stdout);     
    pthread_detach(pthread_self());   
    raspunde((struct thData*)arg);
    close ((intptr_t)arg);
    return(NULL); 
      
};


void raspunde(void *arg)
{
        int nr, i=0;


        char message[300];
        char message1[300];
        
  struct thData tdL; 
  tdL= *((struct thData*)arg);
  int from;
  int to;
if (read (tdL.cl, &from,sizeof(from)) <= 0)
      {
        printf("[Thread ERROR %d]\n",tdL.idThread);
      
      }  
if (read (tdL.cl, &to,sizeof(to)) <= 0)
      {
        printf("[Thread ERROR %d]\n",tdL.idThread);
      
      }  
update_messages(from,to,database);
if (write (tdL.cl, conversations[from][to].conv, 1500) <= 0)
    {
     printf("[Thread ERROR %d] ",tdL.idThread);
    }
char reply_check[20];

while(1)
  {if (read (tdL.cl, message,sizeof(message)) <= 0)
      {
        printf("[Thread ERROR %d]\n",tdL.idThread);
        break;
      
      }      
      time_t rawtime;
    struct tm * timeinfo;
    char numele[50];
    bzero(numele,50);
    strncpy(numele,database[from].name,strlen(database[from].name)-1);
    time ( &rawtime );
    char message_id_char[40];
    timeinfo = localtime ( &rawtime );

    strncpy(reply_check,message,6);
    if (strcmp(reply_check,"reply<")==0)
    {reply(message,from,to);}
    else
    {
    if (strcmp(message," \n")!=0)
    {sprintf(message1, "[%d:%d:%d]:%s| ",timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,numele);
    conversations[from][to].message_id=conversations[from][to].message_id+1;
    sprintf(message_id_char,"<%d>",conversations[from][to].message_id);
    strcat(conversations[from][to].conv,message_id_char);
    strcat(conversations[from][to].conv,message1);
    strcat(conversations[from][to].conv,message);
    strcpy(conversations[to][from].conv,conversations[from][to].conv);
    conversations[to][from].message_id=conversations[from][to].message_id;
    }}


   if (write (tdL.cl, conversations[from][to].conv, 1500) <= 0)
    {
     printf("[Thread %d] ",tdL.idThread);
     break;
    }
 }

}
