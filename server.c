#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <errno.h>
#include "common.h"
#include <arpa/inet.h>
#include "totp.h"
#include<arpa/inet.h>



#define MAX_CLIENT_SUPPORTED 32
#define SERVER_PORT 2000



char data_buffer[1024];
int monitored_fd_set[32];



void init_monitor_fd_set()
{

for(int i=0;i<MAX_CLIENT_SUPPORTED;i++)
monitored_fd_set[i]=-1;

}



void add_to_monitored_fd_set(int skt_fd)
{


for(int i=0;i<MAX_CLIENT_SUPPORTED;i++)
{
  if(monitored_fd_set[i]==-1)
  {
     monitored_fd_set[i]=skt_fd;
     break;
  }

}


}


void remove_from_monitored_fd_set(int skt_fd)
{
for(int i=0;i<MAX_CLIENT_SUPPORTED;i++)
{
   if(monitored_fd_set[i]==skt_fd)
   monitored_fd_set[i]=-1;

}

}


void re_init_readfds(fd_set *fd_set_ptr)
{
   FD_ZERO(fd_set_ptr);
   for(int i=0;i<MAX_CLIENT_SUPPORTED;i++)
   {
     if(monitored_fd_set[i]!=-1)
     FD_SET(monitored_fd_set[i],fd_set_ptr);
   }

}


int get_max_fd()
{

int max=-1;

for(int i=0;i<MAX_CLIENT_SUPPORTED;i++)
{
   if(monitored_fd_set[i]>max)
   max=monitored_fd_set[i];
}

return max;

}




void start_server()
{

int master_socket=0;
int comm_socket=0;
int sent_recv_bytes=0;

fd_set readfds;

struct sockaddr_in server;
struct sockaddr_in client;

init_monitor_fd_set();

if((master_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)
{
  printf("Socket Creation Failed\n");
  exit(1);
}


server.sin_family=AF_INET;
server.sin_port=SERVER_PORT;
server.sin_addr.s_addr=INADDR_ANY;


int addr_len = sizeof(struct sockaddr);

if(bind(master_socket, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        printf("socket bind failed:%d\n",errno);
        return;
    }




 if (listen(master_socket, 5)<0)  
    {
        printf("listen failed\n");
        return;
    }



add_to_monitored_fd_set(master_socket);

while(1){

re_init_readfds(&readfds);   
printf("blocked on select System call...\n");

select(get_max_fd() + 1, &readfds, NULL, NULL, NULL);

if(FD_ISSET(master_socket,&readfds))
{
printf("New Connection Received\n");

comm_socket=accept(master_socket,(struct sockaddr*)&client,&addr_len);

if(comm_socket<0)
{
printf("\nAccept Failed:%d\n",errno);
exit(0);
}


add_to_monitored_fd_set(comm_socket); 
printf("Connection accepted from client : %s:%u\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

}



else
{


int i = 0, comm_socket = -1;
for(; i < MAX_CLIENT_SUPPORTED; i++){

if(FD_ISSET(monitored_fd_set[i], &readfds)){

comm_socket = monitored_fd_set[i];

memset(data_buffer, 0, sizeof(data_buffer));
sent_recv_bytes = recvfrom(comm_socket, (char *)data_buffer, sizeof(data_buffer), 0, (struct sockaddr *)&client, &addr_len);

printf("Server recvd %d bytes from client %s:%u\n", sent_recv_bytes,
inet_ntoa(client.sin_addr), ntohs(client.sin_port));

if(sent_recv_bytes == 0){
close(comm_socket);
remove_from_monitored_fd_set(comm_socket);
break;

}

unsigned int *x = (unsigned int*)data_buffer; 

//printf("\nDATA RECEIVED:%d\n",*x);

 if(x==0){

                        close(comm_socket);
                        remove_from_monitored_fd_set(comm_socket);
                        printf("Server closes connection with client : %s:%u\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                        /*Goto state machine State 1*/
                        break;/*Get out of inner while loop, server is done with this client, time to check for new connection request by executing selct()*/
                    }

  size_t pos;
    size_t len;
    size_t keylen;
    uint8_t *k;
    char *fname = NULL;
    char key[]="JBSWY3DPEHPK3PXP";
    int mode = 0;
    int opt;
    uint32_t result;

 


  
        len = strlen(key);
        k = (uint8_t *)key;
        keylen = decode_b32key(&k, len);
        result = totp(k,keylen);
        
        
        int mssg=0;
        if(result==*x){
        mssg=1;
        }
   
   

 sent_recv_bytes = sendto(comm_socket, (int*)&mssg, sizeof(int), 0,
                            (struct sockaddr *)&client, sizeof(struct sockaddr));

printf("Server sent %d bytes in reply to client\n", sent_recv_bytes);



}




}

}
}



}



int main()
{

start_server();


}
