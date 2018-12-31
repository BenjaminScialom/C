#include "dsm.h"
#include "common_impl.h"

int DSM_NODE_NUM; /* nombre de processus dsm */
int DSM_NODE_ID;  /* rang (= numero) du processus */
dsm_proc_conn_t *conn_array;
int* sockConn;
int tubeThread[2];

/* indique l'adresse de debut de la page de numero numpage */
static char *num2address( int numpage )
{
  char *pointer = (char *)(BASE_ADDR+(numpage*(PAGE_SIZE)));

  if( pointer >= (char *)TOP_ADDR ){
    fprintf(stderr,"[%i] Invalid address !\n", DSM_NODE_ID);
    return NULL;
  }
  else return pointer;
}

static long int address2num(char* addr){
  return ((long int) ((addr-BASE_ADDR))/(PAGE_SIZE));
}

/* fonctions pouvant etre utiles */
static void dsm_change_info( int numpage, dsm_page_state_t state, dsm_page_owner_t owner)
{
  if ((numpage >= 0) && (numpage < PAGE_NUMBER)) {
    if (state != NO_CHANGE )
    table_page[numpage].status = state;
    if (owner >= 0 )
    table_page[numpage].owner = owner;
    return;
  }
  else {
    fprintf(stderr,"[%i] Invalid page number !\n", DSM_NODE_ID);
    return;
  }
}

static dsm_page_owner_t get_owner( int numpage)
{
  return table_page[numpage].owner;
}

static dsm_page_state_t get_status( int numpage)
{
  return table_page[numpage].status;
}

/* Allocation d'une nouvelle page */
static void dsm_alloc_page( int numpage )
{
  char *page_addr = num2address( numpage );
  mmap(page_addr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  return ;
}

/* Changement de la protection d'une page */
static void dsm_protect_page( int numpage , int prot)
{
  char *page_addr = num2address( numpage );
  mprotect(page_addr, PAGE_SIZE, prot);
  return;
}

static void dsm_free_page( int numpage )
{
  char *page_addr = num2address( numpage );
  munmap(page_addr, PAGE_SIZE);
  return;
}

static int dsm_send(int dest,void *buf,size_t size)
{
  return write(dest, buf, size);
  /* a completer */
}

static int dsm_recv(int from,void *buf,size_t size)
{
  return read(from, buf, size);
  /* a completer */
}

static void *dsm_comm_daemon( void *arg)
{

  struct pollfd fds[DSM_NODE_NUM+1];
  int nready;
  int buff;
  int req;
  int numpage;
  int rank;
  int i;

  for(i=0;i<DSM_NODE_NUM;i++){
    fds[i].fd=sockConn[i];
    fds[i].events = POLLIN;
  }

  fds[DSM_NODE_NUM].fd=tubeThread[0];
  fds[DSM_NODE_NUM].events = POLLIN;

  while(1)
  {
    /* a modifier */
    nready = poll(fds,DSM_NODE_NUM+1,-1);

    if(nready>0){
      for(i=0;i<DSM_NODE_NUM;i++){
        if(fds[i].revents==POLLIN){
          read(fds[i].fd,&buff,sizeof(int));

          if(buff==REQ_PAGE){  // Envoi d'une page à un autre processus
          read(fds[i].fd,&buff,sizeof(int));
          req=GET_PAGE;
          dsm_send(fds[i].fd, &req, sizeof(int));
          dsm_send(fds[i].fd, &buff, sizeof(int));  // buff=numpage
          dsm_send(fds[i].fd,num2address(buff), PAGE_SIZE);
          dsm_free_page(buff);
        }
        else if(buff==GET_PAGE){  // Reception d'une page
          dsm_recv(fds[i].fd, &numpage, sizeof(int));
          dsm_recv(fds[i].fd, num2address(numpage), PAGE_SIZE);
          //Envoi d'un message de mis à jour des infos à tous les autres processus
          req=UPDATE_INFO;
          for (i=0;i<DSM_NODE_NUM;i++){
            if(i!=DSM_NODE_ID){
              dsm_send(sockConn[i], &req, sizeof(int));
              dsm_send(sockConn[i], &numpage, sizeof(int));
              dsm_send(sockConn[i], &DSM_NODE_ID, sizeof(int));
            }
          }
        }
        else if(buff==UPDATE_INFO){  // Reception d'une info de Changement propriétaire de page
          dsm_recv(fds[i].fd, &numpage, sizeof(int));
          dsm_recv(fds[i].fd, &rank, sizeof(int));
          dsm_change_info(numpage, NO_CHANGE, rank);
        }
      }

    }
    if(fds[DSM_NODE_NUM].revents==POLLIN){
      pthread_exit(NULL);
    }
  }

//  printf("[%i] Waiting for incoming reqs \n", DSM_NODE_ID);
//  sleep(2);
}
//return;
}



static void dsm_handler( void* page_addr )
{

  int numpage;
  dsm_page_owner_t owner;
  int req;
  int i;
  void *buff;

  numpage = address2num((char*) page_addr);
  owner = get_owner(numpage);

  dsm_alloc_page(numpage);

  //Envoi d'une requête pour récupérer la page
  req=REQ_PAGE;
  dsm_send(sockConn[owner], &req, sizeof(int));
  dsm_send(sockConn[owner], &numpage, sizeof(int));

  dsm_change_info(numpage, NO_CHANGE, DSM_NODE_ID);

  /* A modifier */
  //printf("[%i] FAULTY  ACCESS !!! \n",DSM_NODE_ID);
  //abort();

}

/* traitant de signal adequat */
static void segv_handler(int sig, siginfo_t *info, void *context)
{
  /* A completer */
  /* adresse qui a provoque une erreur */
  void  *addr = info->si_addr;
  /* Si ceci ne fonctionne pas, utiliser a la place :*/
  /*
  #ifdef __x86_64__
  void *addr = (void *)(context->uc_mcontext.gregs[REG_CR2]);
  #elif __i386__
  void *addr = (void *)(context->uc_mcontext.cr2);
  #else
  void  addr = info->si_addr;
  #endif
  */
  /*
  pour plus tard (question ++):
  dsm_access_t access  = (((ucontext_t *)context)->uc_mcontext.gregs[REG_ERR] & 2) ? WRITE_ACCESS : READ_ACCESS;
  */
  /* adresse de la page dont fait partie l'adresse qui a provoque la faute */
  void  *page_addr  = (void *)(((unsigned long) addr) & ~(PAGE_SIZE-1));

  if ((addr >= (void *)BASE_ADDR) && (addr < (void *)TOP_ADDR))
  {
    printf("SEGFAULT\n");
    dsm_handler(page_addr);
  }
  else
  {
    /* SIGSEGV normal : ne rien faire*/
  }
}

/* Seules ces deux dernieres fonctions sont visibles et utilisables */
/* dans les programmes utilisateurs de la DSM                       */
char *dsm_init(int argc, char **argv)
{
  struct sigaction act;
  int index;
  int i;
  int lanceurFd;
  int buffRank;
  int buffPort;
  char* buff;
  int port_num;
  struct sockaddr_in addr_in;
  socklen_t addrlen;

  lanceurFd=atoi(argv[argc-1]);

  /* Creation de la socket d'ecoute pour les */
  /* connexions avec les autres processus dsm */
  int acceptSock=creer_socket(0,&port_num);
  do_listen(acceptSock,20);

  /* Envoi du numero de port au lanceur */
  /* pour qu'il le propage à tous les autres */
  /* processus dsm */
  write(lanceurFd,&port_num,sizeof(int));



  /* reception du nombre de processus dsm envoye */
  /* par le lanceur de programmes (DSM_NODE_NUM)*/
  read(lanceurFd, &DSM_NODE_NUM, sizeof(int));
  conn_array=malloc(DSM_NODE_NUM*sizeof(dsm_proc_conn_t));
  buff=malloc(64*sizeof(char));

  /* reception de mon numero de processus dsm envoye */
  /* par le lanceur de programmes (DSM_NODE_ID)*/
  read(lanceurFd,&DSM_NODE_ID,sizeof(int));

  /* reception des informations de connexion des autres */
  /* processus envoyees par le lanceur : */
  /* nom de machine, numero de port, etc. */
  for (i=0;i<DSM_NODE_NUM;i++){
    read(lanceurFd, &buffRank,sizeof(int));
    conn_array[buffRank].rank=buffRank;

    memset(buff, 0, 64);
    read(lanceurFd, buff, sizeof(char));

    /* 2- puis la chaine elle-meme correspondant au nom de la machine*/
    int taille = atoi(buff);
    memset(buff,0,64);
    read(lanceurFd, buff,taille*sizeof(char));
    strcpy(conn_array[buffRank].name, buff);

    read(lanceurFd, &buffPort, sizeof(int));
    conn_array[buffRank].num_port=buffPort;

  }

  /* initialisation des connexions */
  /* avec les autres processus : connect/accept */
  sockConn=malloc(sizeof(int)*DSM_NODE_NUM);
  int sockfd;
  int rank;
  for (i=0;i<DSM_NODE_ID;i++){
    //sockConn[i]= accept(acceptSock,(struct sockaddr*)&addr_in,&addrlen);
    sockfd = accept(acceptSock,(struct sockaddr*)&addr_in,&addrlen);
    //if(sockConn[i] == -1){
    if(sockfd == -1){
      ERROR_EXIT("Accept");
    }
    else{
      read(sockfd, &rank, sizeof(int));
      sockConn[rank]=sockfd;
    }

  }

  for(i=DSM_NODE_ID+1;i<DSM_NODE_NUM;i++){
    struct hostent* res;
    res = gethostbyname(conn_array[i].name);

    // port num: argv[2]
    struct sockaddr_in* addr=malloc(sizeof(struct sockaddr_in));
    memset(addr,0,sizeof(struct sockaddr_in));
    addr->sin_family=AF_INET;
    addr->sin_port=htons(conn_array[i].num_port);
    addr->sin_addr=*((struct in_addr*) res->h_addr_list[0]);


    // Gestion des erreurs de connexion
    sockConn[i]=do_socket(AF_INET,SOCK_STREAM,0);
    if(connect(sockConn[i],(struct sockaddr*)addr,sizeof(struct sockaddr))== -1){
      ERROR_EXIT("Connect");
    }
    else{
      write(sockConn[i],&DSM_NODE_ID, sizeof(int));
    }

  }

  /* Allocation des pages en tourniquet */
  for(index = 0; index < PAGE_NUMBER; index ++){
    if ((index % DSM_NODE_NUM) == DSM_NODE_ID)
    dsm_alloc_page(index);
    dsm_change_info( index, WRITE, index % DSM_NODE_NUM);
  }

  /* mise en place du traitant de SIGSEGV */
  act.sa_flags = SA_SIGINFO;
  act.sa_sigaction = segv_handler;
  sigaction(SIGSEGV, &act, NULL);

  /* creation du thread de communication */
  /* ce thread va attendre et traiter les requetes */
  /* des autres processus */
  pipe(tubeThread);
  pthread_create(&comm_daemon, NULL, dsm_comm_daemon, NULL);

  /* Adresse de début de la zone de mémoire partagée */
  return ((char *)BASE_ADDR);
}

void dsm_finalize( void )
{
  /* fermer proprement les connexions avec les autres processus */

  /* terminer correctement le thread de communication */
  /* pour le moment, on peut faire : */
  //pthread_cancel(comm_daemon);
  int end;
  end = 0;
  write(tubeThread[1],&end,sizeof(int));

  return;
}
