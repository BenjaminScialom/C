#include "common_impl.h"

int do_socket(int domaine,int type,int protocol){
  int fd = socket(domaine,type,protocol);
  if (fd != -1){
    return fd;
  }
  else {
    ERROR_EXIT("Socket");
  }
}



void initialisation_serveur(struct sockaddr_in* addr){
  memset(addr,0,sizeof(struct sockaddr_in));
  addr->sin_family=AF_INET;
  addr->sin_addr.s_addr=htonl(INADDR_ANY);
  addr->sin_port=htons(0);
}

void do_bind(int fd, struct sockaddr_in addr_in){
  struct sockaddr* addr = (struct sockaddr*)&addr_in;
  int b=bind(fd,addr,sizeof(addr_in));
  if (b == -1){
    ERROR_EXIT("Bind");
  }
}

void do_listen(int fd, int nb_proc){
  int l= listen(fd,nb_proc);
  if (l == -1){
    ERROR_EXIT("Listen");
  }
}

int creer_socket(int prop, int *port_num)
{
   int fd = 0;
   struct sockaddr_in addr;
   initialisation_serveur(&addr);
   socklen_t addrlen = sizeof(struct sockaddr);
   /* fonction de creation et d'attachement */
   /* d'une nouvelle socket */
   /* renvoie le numero de descripteur */
   fd=do_socket(AF_INET,SOCK_STREAM,0);//IPPROTO_TCP);

   do_bind(fd, addr);

   /* et modifie le parametre port_num */
   if(getsockname(fd,(struct sockaddr *) &addr, &addrlen)==0){
      *port_num = ntohs(addr.sin_port);
    }
    else{
      ERROR_EXIT("getsockname");
    }


   return fd;
}

/* Vous pouvez ecrire ici toutes les fonctions */
/* qui pourraient etre utilisees par le lanceur */
/* et le processus intermediaire. N'oubliez pas */
/* de declarer le prototype de ces nouvelles */
/* fonctions dans common_impl.h */
