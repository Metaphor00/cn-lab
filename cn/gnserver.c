#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/times.h>
#include<sys/types.h>
#include<netdb.h>
#define MAX 80
#define PORT 8080

struct timeval timeout;
void func(int connfd)
{
    char buff[MAX];
    int f,c,ack,next = 0;
    while(1)
    {
        sleep(1);
        bzero(buff,MAX);
        recv(connfd,buff,MAX,0);
        if(strcmp("Exit",buff)==0)
        {
            printf("exit..\n");
            break;
        }
        f=atoi(buff);
        if(f!=next)
        {
            printf("frame %d discarded\n",f);
            bzero(buff,MAX);
            snprintf(buff,sizeof(buff),"%d",ack);
            send(connfd,buff,sizeof(buff),0);
            continue;
        }
        c=rand()%3;
        switch(c)
        {
            case 0:
            //printf("frame %d not received\n");
            break;
            case 1:
                ack=f;
                sleep(2);
                printf("frame %d received\nacknowledgement sent: %d \n",f,ack);
                bzero(buff,MAX);
                snprintf(buff,sizeof(buff),"%d",ack);
                send(connfd,buff,sizeof(buff),0);
                next = ack+1;
                break;
            case 2:
                ack=f;
                printf("frame %d received\nacknowledgement sent: %d \n",f,ack);
                bzero(buff,MAX);
                snprintf(buff,sizeof(buff),"%d",ack);
                send(connfd,buff,sizeof(buff),0);
                next = ack+1;
                break;
        }

    }
}
int main(void)
{
    int socket_desc,client_sock,client_size;
    struct sockaddr_in server_addr,client_addr;
    //1.creating the socket
    socket_desc=socket(AF_INET,SOCK_STREAM,0);
    if(socket_desc<0)
    {
        printf("error in socket creation..\n");
        return -1;
    }
    
    printf("socket created successfully...\n");

    bzero(&server_addr,sizeof(server_addr));
    
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(PORT);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    //binding
    if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        printf("socket bind failed..\n");
        return -1;
    }
    printf("socket bind successfully...\n");
    //listen
    if(listen(socket_desc,1)<0)
    {
        printf("listen failed\n");
        return -1;
    }
    printf("server listening.....\n");
    //accept
    client_size=sizeof(client_addr);
    client_sock=accept(socket_desc,(struct sockaddr*)&client_addr,&client_size);
    if(client_sock<0)
    {
        printf("error in accepting..\n");
        return -1;
    }
    printf("server accept the client...\n");
    func(client_sock);
    close(socket_desc);
    return 0;

}
