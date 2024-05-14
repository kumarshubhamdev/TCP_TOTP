#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include "common.h"
#include "CSS.h"
#include<assert.h>


#define DEST_PORT            2000
#define SERVER_IP_ADDRESS   "194.135.92.193"


void start_client()
{

int sock_fd=0;
int sent_recv_bytes=0;
int addr_len=0;


addr_len=sizeof(struct sockaddr);


struct sockaddr_in dest;


dest.sin_family=AF_INET;
dest.sin_port = DEST_PORT;
struct hostent *host = (struct hostent *)gethostbyname(SERVER_IP_ADDRESS);
dest.sin_addr = *((struct in_addr *)host->h_addr);


sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
connect(sock_fd, (struct sockaddr *)&dest,sizeof(struct sockaddr));




int flag=1;
while(flag){
unsigned int client_data; scanf("%d",&client_data);

sent_recv_bytes = sendto(sock_fd,&client_data,sizeof(struct data_t),0,(struct sockaddr *)&dest,sizeof(struct sockaddr));

int res=0;

sent_recv_bytes =  recvfrom(sock_fd, (int *)&res, sizeof(int), 0,
                   (struct sockaddr *)&dest, &addr_len);
                   
if(res==1){
printf(ANSI_COLOR_GREEN"\nCorrect OTP\n"ANSI_COLOR_RESET);
flag=0;
}

else
{
printf(ANSI_COLOR_RED"\nIncorrect OTP try again..\n"ANSI_COLOR_RESET);
}


}
}


int main()
{
start_client();
return 0;
}