#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>



/* autres includes (eventuellement) */

#define ERROR_EXIT(str) {perror(str);exit(EXIT_FAILURE);}

/* definition du type des infos */
/* de connexion des processus dsm */
struct dsm_proc_conn  {
   int rank;
   char name[64];
   int num_port;
};
typedef struct dsm_proc_conn dsm_proc_conn_t;

/* definition du type des infos */
/* d'identification des processus dsm */
struct dsm_proc {
  pid_t sonPid;
  pid_t distPid;
  dsm_proc_conn_t connect_info;
  int pipefd_out;
  int pipefd_err;
  int initSock;
};
typedef struct dsm_proc dsm_proc_t;

int do_socket(int domaine,int type,int protocol);
void initialisation_serveur(struct sockaddr_in* addr);
void do_bind(int fd, struct sockaddr_in addr_in);
void do_listen(int fd, int nb_proc);
int creer_socket(int type, int *port_num);
