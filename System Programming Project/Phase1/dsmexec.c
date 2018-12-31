#include "common_impl.h"

/* variables globales */


/* un tableau gerant les infos d'identification */
/* des processus dsm */
dsm_proc_t *proc_array = NULL;

/* le nombre de processus effectivement crees */
volatile int num_procs_creat = 0;

void usage(void)
{
  fprintf(stdout,"Usage : dsmexec machine_file executable arg1 arg2 ...\n");
  fflush(stdout);
  exit(EXIT_FAILURE);
}
//////////////////////////////////////////////////////////////////traitant_signal////////////////////////////////////////////////////////////////////////////////////////
void sigchld_handler(int sig)
{

  fflush(stdout);
  pid_t pid;
  pid=wait(NULL);
  /* on traite les fils qui se terminent */
  /* pour eviter les zombies */

  int i;
  for (i=0;i<num_procs_creat;i++){
    if(proc_array[i].sonPid == pid){
      close(proc_array[i].pipefd_out);
      close(proc_array[i].pipefd_err);
      close(proc_array[i].initSock);

    }
  }
  num_procs_creat--;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////main///////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  if (argc < 3){
    usage();
  } else {
    pid_t pid;
    int num_procs = 0;
    int i;
    int j;
    int fd;
    int pipefd_out[2];
    int pipefd_err[2];
    struct sockaddr_in addr_in;
    socklen_t addrlen;
    int distSock;
    char* buff;
    struct sigaction sig_act;
    sigset_t *set;
    sigset_t *invset;

    /* Mise en place d'un traitant pour recuperer les fils zombies*/
    memset(&sig_act,0,sizeof(struct sigaction));
    sig_act.sa_handler = sigchld_handler;
    sigaction(SIGCHLD,&sig_act,NULL);

    set = malloc(sizeof(sigset_t));
    sigemptyset(set);
    sigaddset(set, SIGCHLD);
    sigprocmask(SIG_BLOCK, set, NULL);


    /* lecture du fichier de machines */
    FILE* f=fopen(argv[1],"r");
    char name[64];
    char point_char[1];
    j=0;
    memset(name,0,64);
    /* 1- on recupere le nombre de processus a lancer*/
    /* 2- on recupere les noms des machines : le nom de */
    do {
      /* code */
      fread(point_char,sizeof(char),1,f);

      if(point_char[0]=='\n'){
        if(name[0]!=0){

          dsm_proc_conn_t* dsm_proc_conn = malloc(sizeof(dsm_proc_conn_t));
          //dsm_proc_conn->rank=num_procs;
          strcpy(dsm_proc_conn->name, name);

          dsm_proc_t* dsm_proc = malloc(sizeof(dsm_proc_t));
          dsm_proc->connect_info = *dsm_proc_conn;
          dsm_proc->sonPid=-1;
          dsm_proc->distPid=-1;
          num_procs+=1;

          proc_array = realloc(proc_array, num_procs*sizeof(dsm_proc_t));
          proc_array[num_procs-1] = *dsm_proc;


        }
        memset(name,0,64);
        j=0;
      }
      else{
        name[j]=point_char[0];
        j++;
      }
    } while(feof(f)==0);
    fclose(f);

    /*Affichage des noms de machines */
    /* la machine est un des elements d'identification */
/*
    for(i = 0; i < num_procs ; i++) {
      printf("%s\n", proc_array[i].connect_info.name);
    }
*/
    /*Affichage du nombre de processus */
//    printf("le nombre est: %d\n",num_procs);


    /* creation de la socket d'ecoute */
    int port_num;
    fd=creer_socket(0,&port_num);
//    printf("numero de port:%d\n", port_num);
    /* + ecoute effective */
    do_listen(fd,num_procs);
    /* creation des fils */
    for(i = 0; i < num_procs ; i++) {

      /* creation du tube pour rediriger stdout */
      if(pipe2(pipefd_out,O_NONBLOCK)!= 0){
        ERROR_EXIT("Pipe_out");
      }
      /* creation du tube pour rediriger stderr */
      if(pipe2(pipefd_err,O_NONBLOCK) != 0){
        ERROR_EXIT("Pipe_err");
      }

      pid = fork();
      if(pid == -1) ERROR_EXIT("fork");

      if (pid == 0) { /* fils */

        /* redirection stdout */
        close(pipefd_out[0]);
        close(STDOUT_FILENO);
        dup(pipefd_out[1]);
        close(pipefd_out[1]);
        /*redirection stderr*/
        close(pipefd_err[0]);
        close(STDERR_FILENO);
        dup(pipefd_err[1]);
        close(pipefd_err[1]);


        /* Creation du tableau d'arguments pour le ssh */
        int k;
        char* newargv[argc+4];
        newargv[0]= "ssh";
        newargv[1]= proc_array[i].connect_info.name;
        newargv[2]= "/net/t/bscialom/Documents/PR204/Phase1/bin/dsmwrap";
        newargv[3] = malloc(64);
        gethostname(newargv[3],64);
        newargv[4] = malloc(32);
        sprintf(newargv[4],"%d",port_num);

        for(k=0;k<argc-2;k++){
          newargv[5+k]= malloc(128);
          strcpy(newargv[5+k],argv[2+k]);
        }
        newargv[argc+3]=NULL;

        /* jump to new prog : */
        fflush(stdout);///////////////
        fflush(stderr);
        execvp("ssh",newargv);

      } else  if(pid > 0) { /* pere */
        /* fermeture des extremites des tubes non utiles */
        close(pipefd_out[1]);
        close(pipefd_err[1]);

        proc_array[i].pipefd_out=pipefd_out[0];
        proc_array[i].pipefd_err=pipefd_err[0];
        proc_array[i].sonPid=pid;
        num_procs_creat++;
      }
    }

    buff = malloc(sizeof(char)*64);
    //addr_in = malloc(sizeof(struct sockaddr_in));
    //addrlen = malloc(sizeof(socklen_t));pid=wait();

    for(i = 0; i < num_procs ; i++){
      /* on accepte les connexions des processus dsm */
      distSock= accept(fd,(struct sockaddr*)&addr_in,&addrlen);
      if(distSock == -1){
        ERROR_EXIT("Accept");
      }
      /*  On recupere le nom de la machine distante */

      /* 1- d'abord la taille de la chaine */
      memset(buff, 0, 64);
      read(distSock, buff, sizeof(char));
    //  printf("%s\n", buff);

      /* 2- puis la chaine elle-meme correspondant au nom de la machine*/
      int taille = atoi(buff);
      memset(buff,0,64);
      read(distSock, buff,taille*sizeof(char));
    //  printf("%s\n",buff);

      /* On recupere le pid du processus distant  */
      pid_t distPid;
      read(distSock,&distPid,sizeof(pid_t));
    //  printf("%d\n",distPid);

      /* On recupere le numero de port de la socket */
      /* d'ecoute des processus distants */
      int distPort;
      read(distSock,&distPort,sizeof(int));
    //  printf("%d\n",distPort);

      for (j=0;j<num_procs;j++){
        if(strcmp(proc_array[j].connect_info.name,buff)==0 && proc_array[j].distPid==-1){
          proc_array[j].distPid=distPid;
          proc_array[j].connect_info.num_port=distPort;
          proc_array[j].initSock=distSock;
          proc_array[j].connect_info.rank=j;
          break;
        }
      }
    }



    for (i=0;i<num_procs;i++){
      /* envoi du nombre de processus aux processus dsm*/
      write(proc_array[i].initSock,&num_procs,sizeof(int));
      /* envoi des rangs aux processus dsm */
      write(proc_array[i].initSock,&(proc_array[i].connect_info.rank),sizeof(int));

      /* envoi des infos de connexion aux processus */
      for (j=0;j<num_procs;j++){
        /* envoi des rangs aux processus dsm */
        write(proc_array[i].initSock,&(proc_array[j].connect_info.rank),sizeof(int));
        /* envoi des noms de machines aux processus dsm */

        sprintf(buff,"%zi%s", strlen(proc_array[j].connect_info.name), proc_array[j].connect_info.name);
        int to_send = strlen(buff);
        int sent = 0;
        do{
          sent += write(proc_array[i].initSock, buff + sent , to_send - sent);
        } while(sent != to_send);

        /* envoi des numéros de ports aux processus dsm */
        write(proc_array[i].initSock,&(proc_array[j].connect_info.num_port),sizeof(int));
      }
    }


    /* gestion des E/S : on recupere les caracteres */
    /* sur les tubes de redirection depid=wait(); stdout/stderr */
    int PIPE_SIZE=5000;
    char* buffer=malloc(sizeof(char)*PIPE_SIZE);
    int nb_lu;


    while(1)
    {
      /*je recupere les infos sur les tubes de redirection
      jusqu'à ce qu'ils soient inactifs (ie fermes par les
      processus dsm ecrivains de l'autre cote ...)
      */
      for (i=0;i<num_procs;i++){
        memset(buffer,0,PIPE_SIZE);
        nb_lu=read(proc_array[i].pipefd_out,buffer,PIPE_SIZE);
        if(nb_lu>0){
          printf("[Proc %d: %s : stdout] %s\n",proc_array[i].connect_info.rank, proc_array[i].connect_info.name,buffer);
        }
        memset(buffer,0,PIPE_SIZE);
        nb_lu=read(proc_array[i].pipefd_err,buffer,PIPE_SIZE);
        if(nb_lu>0){
          printf("[Proc %d: %s : stdout] %s\n",proc_array[i].connect_info.rank, proc_array[i].connect_info.name,buffer);
        }
      }
      if(nb_lu==0){
        break;
      }
    };

    sigprocmask(SIG_UNBLOCK, set, NULL);

    invset = malloc(sizeof(sigset_t));
    sigfillset(invset);
    sigdelset(invset, SIGCHLD);

    while(0){
      sigprocmask(SIG_BLOCK, set, NULL);
      if(num_procs_creat>0){
        sigsuspend(invset);
      }
      else{
        break;
      }
      sigprocmask(SIG_UNBLOCK, set, NULL);
}


    /* on ferme les descripteurs proprement */
    /* on ferme la socket d'ecoute */
    close(distSock);
  }
  exit(EXIT_SUCCESS);
}
