#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PORT 6000


extern int errno;

int command_type(char *command) // tipul de commanda in  meniul principal si secundar
{
 if (strcmp(command,"login")==0)
    return 0;
 if (strcmp(command,"register")==0)
    return 1;
 if (strcmp(command,"logout")==0)
    return 2;
  if (strcmp(command,"quit")==0)
    return 3;
  if (strcmp(command,"userslist")==0)
    return 4;
  if (strcmp(command,"sendrequest")==0)
    return 5;
  if (strcmp(command,"back")==0)
    return 6;
  if (strcmp(command,"exit")==0)
    return 7;
  if (strcmp(command,"friendrequest")==0)
    return 8;
  if (strcmp(command,"friendlist")==0)
    return 9;
return -1;
}
struct users{
      int id;
      char name[100];
      int descriptor;
              };

int name_to_id(const char * nume,struct users database[])                                   //convertor nume user in id user
{
  char decautat[100];
  strcpy(decautat,nume);
  strcat(decautat,"\n");
  int j=1;
  int pozj;
  while(database[j].id!=0)
  {
    if(strcmp(decautat,database[j].name)==0)
          {return j;}
  j=j+1;}
  return 0;}
int command_type_fr1(char *command)                                                         //tip de commanda accept/refuse in meniul pentru friendrequest
{
  char type_command[10];
  strncpy(type_command,command,6);
  if (strcmp(type_command,"accept")==0)
    return 1;
  if (strcmp(type_command,"refuse")==0)
    return 2;
 return -1;}
int command_type_fr2(char *command,struct users database[])                                 //tip de commanda pentru all/sau un id concret din structura database
{
  char type_command[10];
  strcpy(type_command,command);
  strcpy(type_command,type_command+7);
  if (strcmp(type_command,"all")==0)
    return -10;
  int j=-10;
  j=name_to_id(type_command,database);
 return j;}
int command_type_fr3(char *command)                                                         //tip de commanda delete/refuse in meniul petru friendrequest
{
  char type_command[10];
  strncpy(type_command,command,6);
  if (strcmp(type_command,"delete")==0)
    return 1;
  if (strcmp(type_command,"refuse")==0)
    return 2;
 return -1;}
int command_type_fr4(char *command,struct users database[])                                 //tip de commanda pentru meniul friendlist care returna id existent din database
{
  char type_command1[50];
  char type_command2[50];
  int j=-10;
  strcpy(type_command1,command);
  strncpy(type_command2,command,13);
  if (strcmp(type_command2,"send_message_")==0)
     {bzero(type_command2,50);
      strcpy(type_command2,type_command1+13);
     j=name_to_id(type_command2,database);}

 return j;}
void build_structure(struct users database[])                                               //void pentru generearea structurii din fisierul cu conturi
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
int n_matrix(struct users database[])                                                       //returneaza numarul de users din database
{
  int j=1;
  while(database[j].id!=0)
  {j=j+1;}
  return j-1;}
void build_descriptors(struct users database[],int q, int y)                                //flagul de online/offline pentru fiecare din useri
{
  int i;
  FILE *file1;
  file1=fopen("descriptors.config", "r");
 int n=n_matrix(database);
 for(i = 1; i <=n; i++)
  {

       if (!fscanf(file1, "%d", &database[i].descriptor)) 
           break;
  }
  fclose(file1);

  FILE *file2;
  file2=fopen("descriptors.config", "w");
 
 for(i = 1; i <=n; i++)
  {

        if (i!=q)
         fprintf(file2,"%d \n",database[i].descriptor);
        else
          fprintf(file2,"%d \n",y);
  }
  fclose(file2);
  database[q].descriptor=y;}
void read_matrix(int matrix[100][100])                                                      //citeste matricea de tangente din fisier
{ 
  int i;
  int j;
  int n;
  FILE *file;
  file=fopen("matrix.in", "r");
 fscanf(file, "%d", &n);
 for(i = 1; i <=n; i++)
  {
      for(j = 1; j <=n; j++) 
      {
       if (!fscanf(file, "%d", &matrix[i][j])) 
           break;
      }

  }
  fclose(file);}
int check_friendreq(const char * nume,struct users database[],int matrix[100][100])         //returneaza notificarea de friendrequest
{ 
  int j=name_to_id(nume,database);
  int n=n_matrix(database);
  int k=1;
  while (k<=n)
    {if (matrix[k][j]==2)
          return 1;
      k=k+1;
    }
    return 0;}
void frql(const char * nume,struct users database[],int matrix[100][100],char list[1000])   //returneaza lista de friendrequesturi pentru userul curent
{ 
  int n=n_matrix(database);
  int i=name_to_id(nume,database);
  int k=1;
  int ok=0;
  while(k<=n)
  {
    if (matrix[k][i]==2)
        {strcat(list,database[k].name); ok=1;}
      k=k+1;
  }
  if (ok==0)
    strcpy(list,"There are no any friend requests.\n");}
int check_messages(int k,int z)                                                             //verifica daca un user a trimis mesaj clientului
{
    int i;
      while (k>0)
      {
        if (z==k%10)
          return 1;
        k=k/10;
      }
      return 0;}
void friend_list_generate(const char * nume,struct users database[],int matrix[100][100],char list[1000]) //genereaza lista curenta de prieteni al clientului
{ 
  int n=n_matrix(database);
  int i=name_to_id(nume,database);
  int k=1;
  int ok=0;
  FILE *file1;
  int j;
  int x;
  file1=fopen("messages.config", "r");
 for(j = 1; j <=i; j++)
  {

       if (!fscanf(file1, "%d", &x)) 
           break;
  }
  fclose(file1);




  while(k<=n)
  {
    if (matrix[k][i]==1)
        {
          if(check_messages(x,k)==1)
          {strcat(list,"*");strcat(list,database[k].name); }
          else
          strcat(list,database[k].name); 


        ok=1;}
      k=k+1;
  }
  if (ok==0)
    strcpy(list,"Your friendlist is empty.\n");}
void update_messages(int q, int k,struct users database[])                                  //notifica un prieten ca ii trimiti un mesaj
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
 v[q]=v[q]*10+k;
 for(i = 1; i <=n; i++)
  {

        
         fprintf(file2,"%d \n",v[i]);
        
  }
  fclose(file2);}
int check_friendlist(int q,struct users database[])                                         //notifica client ca are un mesaj
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
  if (v[q]!=0)
    return 1;
  else
    return 0;
  fclose(file1);}
void build_matrix(int matrix[100][100],struct users database[])                             //redacteaza matricea din fisier   
{
  int i;
  int j;
  int auxn;
  build_structure(database);
  int n=n_matrix(database);
  n=n+1;
  FILE *file;
  file=fopen("matrix.in", "r");
  fscanf(file, " %d", &auxn);
  fclose(file);
 if (n>=auxn)
 {
  FILE *f1=fopen("matrix.in","w");
  fprintf(f1,"%d\n",n);
  
 for(i = 1; i <= n; i++)
  {
      for(j = 1; j <= n; j++) 
      {
       if(j>=auxn || i>=auxn)
        matrix[i][j]=0;
       fprintf(f1,"%d ",matrix[i][j]);
      }
 fprintf(f1,"\n");
 }


  fclose(f1);}}
void update_matrix(int matrix[100][100],struct users database[])                            //update complet pentru matrice
{ 
  read_matrix(matrix);
  build_matrix(matrix,database);
  read_matrix(matrix);}
void update_matrix1(int matrix[100][100],struct users database[])                           //update partial pentru menu friendrequest
{ 
  build_matrix(matrix,database);
  read_matrix(matrix);}
void update_matrix2(int matrix[100][100],struct users database[])                           //update  partila pentru menu friendlist
{ 
  build_structure(database);
  read_matrix(matrix);}
int username_check(const char * username)                                                   //test pentru username daca e disponibil si daca respecta conditiile impuse
{

    if(strlen(username)<4)
      return 1;

   FILE *fpacc;
   char temp[100];
   char aux[100];
   strcpy(aux,username);
   strcat(aux,"\n");
   if((fpacc = fopen("accounts.config","r"))==NULL) // deschid fisierul cu conturi
        perror("Open accounts file error");
   while(fgets(temp,sizeof(temp),fpacc)!=NULL) // cautam username-ul
        {
        if (strcmp(temp,aux)==0) 
          return 2;
      }
   return 0;}
int password_check(const char * password,const char * cpassword)                            //test pentru password daca respecta conditiile impuse
{
  if(strlen(password)<4)
    return 1;
  if(strcmp(password,cpassword))
    return 2;
  return 0;}
int write_user(const char *username, const char * password)                                 //scrierea userului in fisierele .config
{
  FILE *fpacc;
  FILE *fppass;
  FILE *fpmess;
  if((fpacc = fopen("accounts.config","a"))==NULL) // deschid fisierul cu conturi
        perror("Open accounts file error");
  if((fppass = fopen("passwords.config","a"))==NULL) // deschid fisierul cu parole
        perror("Open passwords file error");
  if((fpmess = fopen("messages.config","a"))==NULL) // deschid fisierul cu parole
        perror("Open passwords file error");
  
  char concats[200];
  char concats1[200];
  strcpy(concats,username);
  strcat(concats,password);
  strcat(concats,"\n");
  strcpy(concats1,username);
  strcat(concats1,"\n");
    fprintf(fpacc, concats1);
    fprintf(fppass, concats);
    fprintf(fpmess,"0 \n");
    fclose(fpacc);
    fclose(fppass);\
    fclose(fpmess);}
int register_username( int client)                                                          //protocolul register pentru un nou user
{
  char username[]="USERNAME: ";
  char password[]="PASSWORD: ";
  char cpassword[]="CONFIRM PASSWORD: ";
  char rusername[100];
  char rpassword[100];
  char rcpassword[100];
  int succes=0;
  int first_time=0;
  char register_menu[300];
  char register_menu_fail_username_too_short[300];
  char register_menu_fail_username_already_in_use[300];
  char register_menu_fail_password_too_short[300];
  char register_menu_fail_passwords_not_matching[300];
  sprintf(register_menu,"Register\n-Username must not have less than 4 characters\n-Password must not have less than 4 caracters\nUSERNAME: ");
  sprintf(register_menu_fail_username_too_short,"Register\n-Username must not have less than 3 characters\n-Password must not have less than 6 caracters\nFAILED: username too short\nUSERNAME: ");
  sprintf(register_menu_fail_username_already_in_use,"Register\n-Username must not have less than 3 characters\n-Password must not have less than 6 caracters\nFAILED: username already in use\nUSERNAME: ");
  sprintf(register_menu_fail_passwords_not_matching,"Register\n-Username must not have less than 3 characters\n-Password must not have less than 6 caracters\nFAILED: passwords not matching\nUSERNAME: ");
  sprintf(register_menu_fail_password_too_short,"Register\n-Username must not have less than 3 characters\n-Password must not have less than 6 caracters\nFAILED: password too short\nUSERNAME: ");
  int error=0;
  if (write (client,register_menu, sizeof(register_menu)) <= 0)
       {
          perror ("[server]Eroare la write() catre client.\n");
          close (client); return 0;
          /* continuam sa ascultam */
       }
    while(succes==0)
    {  if(first_time!=0)
      { if(error==1)
         if (write (client,register_menu_fail_username_too_short, sizeof(register_menu_fail_username_too_short)) <= 0)
             {
                perror ("[server]Eroare la write() catre client.\n");
              close (client); return 0;
              /* continuam sa ascultam */
             }
          if(error==2)
         if (write (client,register_menu_fail_username_already_in_use, sizeof(register_menu_fail_username_already_in_use)) <= 0)
             {
                perror ("[server]Eroare la write() catre client.\n");
              close (client); return 0;
              /* continuam sa ascultam */
             }
      if(error==3)
         if (write (client,register_menu_fail_password_too_short, sizeof(register_menu_fail_password_too_short)) <= 0)
             {
                perror ("[server]Eroare la write() catre client.\n");
              close (client); return 0;
              /* continuam sa ascultam */
             }
      if(error==4)
         if (write (client,register_menu_fail_passwords_not_matching, sizeof(register_menu_fail_passwords_not_matching)) <= 0)
             {
                perror ("[server]Eroare la write() catre client.\n");
              close (client); return 0;
              /* continuam sa ascultam */
             }

      }
       first_time=1;  
       if (read (client, rusername, 100) <= 0)
       {
         perror ("[server]Eroare la read() de la client.\n");
          close (client); return 0; /* inchidem conexiunea cu clientul */
        /* continuam sa ascultam */
       }
       if (command_type(rusername)==3)
    return 0;
       if (write (client,password, sizeof(password)) <= 0)
       {
          perror ("[server]Eroare la write() catre client.\n");
          close (client); return 0;
          /* continuam sa ascultam */
       }  
       if (read (client, rpassword, 100) <= 0)
       {
         perror ("[server]Eroare la read() de la client.\n");
          close (client); return 0;/* inchidem conexiunea cu clientul */
        /* continuam sa ascultam */
       }
       if (write (client,cpassword, sizeof(cpassword)) <= 0)
       {
          perror ("[server]Eroare la write() catre client.\n");
          close (client); return 0;
          /* continuam sa ascultam */
       }  
       if (read (client, rcpassword, 100) <= 0)
       {
         perror ("[server]Eroare la read() de la client.\n");
          close (client); return 0;/* inchidem conexiunea cu clientul */
        /* continuam sa ascultam */
       }
    if(username_check(rusername)==1)
         error=1;
      else
          if(username_check(rusername)==2)
          error=2;
         else
        if(password_check(rpassword,rcpassword)==1)
           error=3;
          else
      if(password_check(rpassword,rcpassword)==2)
      
       error=4;
          else
            succes=1;
  }
  if (succes==1)
  write_user(rusername,rpassword);
  return 1;}
int check_login(char *logg, char * pass)                                                    //testul de logare pentru un client
{
  int ok=0;
  FILE *fpacc;
  FILE *fppass;
  char * temp=(char *) malloc (1000*sizeof(char));
  char * logstr=(char *) malloc (1000*sizeof(char));
  char * passstr=(char *) malloc (1000*sizeof(char));
  strcpy(logstr,logg);
  strcpy(passstr,pass); 
  char qux[100];
  strcpy(qux,logstr);
  strcat(qux,passstr);
  strcat(qux,"\n");
  if((fpacc = fopen("accounts.config","r"))==NULL) // deschid fisierul cu conturi
        perror("Open accounts file error");
  if((fppass = fopen("passwords.config","r"))==NULL) // deschid fisierul cu parole
        perror("Open passwords file error");
  while(fgets(temp,strlen(logstr)+1,fpacc)!=NULL) // cautam username-ul
        {
        if (strcmp(temp,logstr)==0) 
          ok=1;
      }
  if (ok==1)  // daca a gasit username atunci cautam daca se potriveste parola
    { 
      while(fgets(temp,sizeof(qux),fppass)!=NULL) // cautam password
        {
        if (strcmp(temp,qux)==0) 
          return 1;
        }
    }
  return 0;
  fclose(fpacc);
  fclose(fppass);}
int menu(int client,int matrix[100][100],struct users database[])                           //meniul secundar pentru client
{           int boolog=0;
            char commanda[100];
            char quitchar[100];
            strcpy(quitchar,"23twgrhdny4terdgewt4j.t34..v.4.vrwrw4.vrs.vr.svr4");
            char menu_secundar[500];
            int logged=0;
            char loginul[100];
            char parola[100]; 
            update_matrix(matrix,database);
            char login_message[100]="Log in:\n";
            char login_message1[100]="USERNAME: ";
            char login_message2[100]="PASSWORD: ";
            static char ayx[100];
            if (write (client,login_message, strlen(login_message)) <= 0)
             {
               perror ("ERROR 6\n");
               close (client); return 0;} 
             while (logged==0)
             {  bzero(loginul,100);
               if (write (client,login_message1, strlen(login_message)+1) <= 0)
               {
                 perror ("ERROR 7\n");
                 close (client); return 0;
               }
               if (read (client, loginul, 100) <= 0)
               {
                perror ("ERROR 8\n");
                 close (client);}
               if (command_type(loginul)==7)
                 return 0;
               strcpy(ayx,loginul);
               bzero(parola,100);
               if (write (client,login_message2, strlen(login_message2)+1) <= 0)
               {
                 perror ("ERROR 9\n");
                 close (client); return 0;} 
               if (read (client, parola, 100) <= 0)
               {
                 perror ("ERROR 10\n");
                 close (client); return "0";} 
               logged=check_login(loginul,parola); 
             } 

             char str[100];
             char freq_list[1000];
             char friend_list[1000];
             strcpy(str,ayx);
             int q=name_to_id(loginul,database);
             if(strcmp(str,"0")!=0)
             { 
                boolog=1;
                while(boolog==1)
                {   
                    build_descriptors(database,q,1);
                    read_matrix(matrix);
                    build_structure(database);
                   if (check_friendreq(loginul,database,matrix)==1)
                     {if(check_friendlist(q,database)==1)
                      sprintf(menu_secundar,"Logged in as %s:\n friendrequest*   -you have a new friend request/s\n userslist        -list of all users\n friendlist*      -list with your friends\n logout           -if you want to login into another account\n quit             -to exit application\n:> ",loginul);
                     else
                      sprintf(menu_secundar,"Logged in as %s:\n friendrequest*   -you have a new friend request/s\n userslist        -list of all users\n friendlist*      -list with your friends\n logout           -if you want to login into another account\n quit             -to exit application\n:> ",loginul);
                      }
                   else
                    {if(check_friendlist(q,database)==1)
                     sprintf(menu_secundar,"Logged in as %s:\n friendrequest    -you have no any friend request/s\n userslist        -list of all users\n friendlist*      -list with your friends\n logout           -if you want to login into another account\n quit             -to exit application\n:> ",loginul);
                    else
                     sprintf(menu_secundar,"Logged in as %s:\n friendrequest    -you have no any friend request/s\n userslist        -list of all users\n friendlist       -list with your friends\n logout           -if you want to login into another account\n quit             -to exit application\n:> ",loginul);
                    }
                    if (write (client,menu_secundar, sizeof(menu_secundar)) <= 0)
                    {
                       perror ("ERROR 11\n");
                        close (client); return 1;}  
                    if (read (client, commanda, 100) <= 0)
                    {
                       perror ("ERROR 12\n");
                        close (client); return 1;}
                    if(command_type(commanda)==2)
                      {boolog=0; build_descriptors(database,q,0); return 0;}
                    if(command_type(commanda)==3)
                    {
                      if (write (client,quitchar, sizeof(quitchar)) <= 0)
                      {
                         perror ("ERROR 13\n");
                         close (client);}
                         build_descriptors(database,q,0);
                        return 1;
                     }
                    if(command_type(commanda)==4)
                    { 
                      build_structure(database);
                     char usr_listt[500];
                     strcpy(usr_listt,"All users:\n");
                     int i=1;
                     while(database[i].id!=0)
                     {strcat(usr_listt,database[i].name);
                      i=i+1;}
                     char usr_listt1[500];
                     strcpy(usr_listt1,usr_listt);
                     strcat(usr_listt,"\n sendrequest   -send a message request\n back          -to go to previous menu\n:> ");
                     strcat(usr_listt1,"\n sendrequest   -send a message request\n back          -to go to previous menu\nTO: ");
                     puts(" ");
                     int active2=0;
                     while(active2==0)
                     { if (write (client,usr_listt, sizeof(usr_listt)) <= 0)
                      {
                       perror ("ERROR 14\n");
                         close (client); return 1;}  
                      if (read (client, commanda, 100) <= 0)
                      {
                         perror ("ERROR 15\n");
                          close (client); return 1;}
                     if(command_type(commanda)==5)
                     {  
                        int active3=0;
                        while(active3==0)
                        { 
                          if (write (client,usr_listt1, sizeof(usr_listt1)) <= 0)
                        {
                         perror ("ERROR 16\n");
                         close (client); return 1; }  
                        if (read (client, commanda, 100) <= 0)
                        {
                         perror ("ERROR 17\n");
                          close (client); return 1;}
                          char auxiliar[100];
                          strcpy(auxiliar,commanda);
                          strcat(auxiliar,"\n");
                        i=1;
                        int pozi;
                        while(database[i].id!=0)
                        {if(strcmp(auxiliar,database[i].name)==0)
                          { pozi=i;}
                          i=i+1;}
                          int j=1;
                          char decautat[100];
                          strcpy(decautat,loginul);
                          strcat(decautat,"\n");
                          int pozj;
                        while(database[j].id!=0)
                        {if(strcmp(decautat,database[j].name)==0)
                          {pozj=j;}
                          j=j+1;}
                          read_matrix(matrix);
                          matrix[pozj][pozi]=2;
                          build_matrix(matrix,database);
                        
                    


                        if(command_type(commanda)==6)
                        {active3=1; active2=1;}
 
                        }




                     }
                     if(command_type(commanda)==6)
                        active2=1; }
                    }
                    if(command_type(commanda)==8)
                      {
                        int active4=0;
                          while(active4==0)
                          {
                            read_matrix(matrix);
                            build_structure(database);
                           bzero(freq_list,1000);
                          frql(loginul,database,matrix,freq_list);
                          char frq_menu[1000];
                          strcpy(frq_menu,"This users sent you a friend request:\n");
                          strcat(frq_menu,freq_list);
                          strcat(frq_menu,"\n accept_username     -to add specified user in your friends list\n accept_all          -to add all this users to your friends list\n refuse_username     -to refuse specified user\n refuse_all          -to refuse all users\n back                -to go to previous menu\n:>");
                          if (write (client,frq_menu, sizeof(frq_menu)) <= 0)
                           {
                             perror ("ERROR 18\n");
                             close (client); return 1;
                           }  
                           if (read (client, commanda, 100) <= 0)
                           {
                             perror ("ERROR 19\n");
                             close (client); return 1;}
                           if (command_type(commanda)==6)
                              active4=1;
                           
                           if(command_type_fr1(commanda)==1)
                              {
                                  if(command_type_fr2(commanda,database)==-10)
                                  {
                                    int y=name_to_id(loginul,database);
                                    printf("%d \n",y);
                                    int z=n_matrix(database);
                                    int l=1;
                                    while(l<=z)
                                      { if(matrix[l][y]==2)
                                         {read_matrix(matrix);matrix[l][y]=1; matrix[y][l]=1;build_matrix(matrix,database);}
                                      l=l+1;}
                                
                                  }
                                  if (command_type_fr2(commanda,database)!=0)
                                  {
                                    int y=name_to_id(loginul,database);
                                    printf("%d \n",y);
                                    int x=command_type_fr2(commanda,database);
                                    read_matrix(matrix);
                                    matrix[x][y]=1;
                                    matrix[y][x]=1;
                                    build_matrix(matrix,database);
                                  }
                                    
                              }
                           if(command_type_fr1(commanda)==2)
                              {
                                  if(command_type_fr2(commanda,database)==-10)
                                  {
                                    int y=name_to_id(loginul,database);
                                    printf("%d \n",y);
                                    int z=n_matrix(database);
                                    int l=1;
                                    while(l<=z)
                                      {if(matrix[l][y]==2)
                                        {read_matrix(matrix);matrix[l][y]=0;build_matrix(matrix,database);}
                                      l=l+1;}
                                  }
                                  if (command_type_fr2(commanda,database)!=0)
                                  {
                                      int y=name_to_id(loginul,database);
                                      printf("%d \n",y);
                                      int x=command_type_fr2(commanda,database);
                                       read_matrix(matrix);
                                       matrix[x][y]=0;
                                       build_matrix(matrix,database);
                                  }
                                
                              }
                            
                          }
                  

                       }
                    if(command_type(commanda)==9)
                      {
                        int active5=0;
                          while(active5==0)
                          {
                            read_matrix(matrix);
                            build_structure(database);
                           bzero(friend_list,1000);
                          friend_list_generate(loginul,database,matrix,friend_list);
                          char friend_menu[1000];
                          strcpy(friend_menu,"Friendlist:\n");
                          strcat(friend_menu,friend_list);
                          strcat(friend_menu,"\n send_message_user   -to send a message to a friend\n delete_username     -to delete the user from your friendlist\n back                -to go to previous menu\n:>");
                          if (write (client,friend_menu, sizeof(friend_menu)) <= 0)
                           {
                             perror ("ERROR 20\n");
                             close (client); return 1;
                           }  
                           if (read (client, commanda, 100) <= 0)
                           {
                             perror ("ERROR 21\n");
                             close (client); return 1;}
                           if (command_type(commanda)==6)
                              active5=1;
                           if(command_type_fr3(commanda)==1)
                              {
                                if(command_type_fr2(commanda,database)!=10)
                                  {int j=name_to_id(loginul,database);
                                    int k=command_type_fr2(commanda,database);
                                    printf ("%s-%d %s-%d",loginul,j,commanda,k);
                                    read_matrix(matrix);
                                    matrix[k][j]=0;
                                    matrix[j][k]=0;
                                    build_matrix(matrix,database);
                                  }

                              }
                           if(command_type_fr4(commanda,database)!=-10)
                              { int data[5];
                                bzero(data,sizeof(data));
                                int j=name_to_id(loginul,database);
                                int k=command_type_fr4(commanda,database);
                                if (matrix[k][j]==1)
                                  { data[0]=1; data[1]=j; data[2]=k;

                                    update_messages(k,j,database);
                                   }


                                if (write (client,data, sizeof(data)) <= 0)
                                    {
                                       perror ("ERROR 22\n");
                                       close (client); return 1;
                                    } 
                                   

                              }
                           
                            
                          }
                  

                       }

                }
                 build_descriptors(database,q,0);
             }
             return 1; }
void deserveste(int client)                                                                 //meniul principal pentru client                           
{    

  int matrix[100][100];
  struct users database[100];

    int quit=0;
    int boolog=0;
    char menu_principal[500];
    char quitchar[100];
    strcpy(quitchar,"23twgrhdny4terdgewt4j.t34..v.4.vrwrw4.vrs.vr.svr4");
    char commanda[100];
    strcpy(menu_principal,"Choose one of the following actions:\n login         -if you already have an account\n register      -if you don't have an account\n quit          -to exit application\n:> ");
    while(quit==0)
    {
      if (write (client,menu_principal, sizeof(menu_principal)) <= 0)
       {
          perror ("ERROR 23\n");
          close (client); boolog=0; quit=1;
          
       }  
       if (read (client, commanda, 100) <= 0)
       {
         perror ("ERROR 24\n");
          close (client); boolog=0; quit=1;
       }

       if (command_type(commanda)==0)
        {   
           quit=menu(client,matrix,database);
        }
       if(command_type(commanda)==1)
         {
           if(register_username(client)==1)
            {puts("succes register"); update_matrix(matrix,database);}
           else
            puts("ERROR succes register");}
       if (command_type(commanda)==3)
          {quit=1;
            if (write (client,quitchar, sizeof(quitchar)) <= 0)
                      {
                         perror ("ERROR 25\n");
                         close (client);
                       }
          }

      
          
    }
    
    close(client);}



int main ()
{
  struct sockaddr_in server;	
  struct sockaddr_in from;	
  int sd;			
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
  if (listen (sd, 5) == -1)
    {
      perror ("ERROR 3\n");
      return errno;
    }
 pid_t pid;
 int i=0;
  while(1)
    {
    int client;
    int length=sizeof(from);
    client = accept (sd, (struct sockaddr *) &from, &length);
      if (client < 0)
	{
        perror ("ERROR 4\n");
        continue;
	}
    if ((pid=fork())==0)
	{
	    close(sd);
      int ctype;
      if (read (client, &ctype, sizeof(ctype)) <= 0)
       {
         perror ("ERROR 5\n");
          close (client);
       }
      if (ctype==113122311)
	       deserveste(client);
	    close(client);
	    exit(1);
	}
	else{close(client);}
    }
}	