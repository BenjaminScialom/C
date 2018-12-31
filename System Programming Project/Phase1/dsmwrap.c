#include "common_impl.h"


int main(int argc, char **argv)
{
   pid_t pid;

   /* processus intermediaire pour "nettoyer" */
   /* la liste des arguments qu'on va passer */
   /* a la commande a executer vraiment */

   /* creation d'une socket pour se connecter au */
   /* au lanceur et envoyer/recevoir les infos */
   /* necessaires pour la phase dsm_init */
   int fd=do_socket(AF_INET,SOCK_STREAM,0);

   // machine name: argv[1]
   struct hostent* res;
   res = gethostbyname(argv[1]);

   // port num: argv[2]
   struct sockaddr_in* addr=malloc(sizeof(struct sockaddr_in));
   memset(addr,0,sizeof(struct sockaddr_in));
   addr->sin_family=AF_INET;
   addr->sin_port=htons(atoi(argv[2]));
   addr->sin_addr=*((struct in_addr*) res->h_addr_list[0]);

   // Gestion des erreurs de connexion
  if(connect(fd,(struct sockaddr*)addr,sizeof(struct sockaddr))== -1){
    ERROR_EXIT("Connect");
  }

   /* Envoi du nom de machine au lanceur */
   char* data_ptr = malloc(64*sizeof(char));
   char* hostname = malloc(64*sizeof(char));

   //envoi de la taille & de la chaine de caractètres de la machine
   gethostname(hostname,64);
   sprintf(data_ptr,"%zi%s", strlen(hostname), hostname);
   int to_send = strlen(data_ptr);
   int sent = 0;
   do{
     sent += write(fd, data_ptr + sent , to_send - sent);
   } while(sent != to_send);

   /* Envoi du pid au lanceur */
   pid=getpid();
   write(fd,&pid,sizeof(pid_t));


   /* on execute la bonne commande */
   fflush(stdout);
   fflush(stderr);
   int k;
   char* newargv[argc-1];
   for(k=0;k<argc-3;k++){
     newargv[k]= argv[3+k];
   }
   newargv[argc-3]=malloc(128);
   sprintf(newargv[argc-3],"%d",fd);
   newargv[argc-2]=NULL;
   /* jump to new prog : */
   execvp(argv[3],newargv);

   printf("Exec error\n");
   fflush(stdout);

   return 0;
}
