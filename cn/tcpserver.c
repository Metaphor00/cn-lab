#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(void)
{
    //1.declaring variables
    int socket_desc,client_sock,client_size;
    struct sockaddr_in server_addr,client_addr;
    char server_message[2000],client_message[2000];
    //2.clearing the memory
    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));
    //3.creating the socket
    socket_desc=socket(AF_INET,SOCK_STREAM,0);
    if(socket_desc<0)
    {
        printf("error creating the socket...\n");
        return -1;
    }
    printf("socket created successfullly....\n");
    //4.setting the ip address, port,and family
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    //5.binding the port
    if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        printf("error in binding..\n");
        return -1;
    }
    printf("done with binding..\n");
    //6.listening for incoming connections from the client
    client_size=sizeof(client_addr);
    if(listen(socket_desc,1)<0)
    {
        printf("error in listening...\n");
        return -1;
    }
    printf("listening for incoming connections...\n");
    //7.accept the incoming connections
    client_sock=accept(socket_desc,(struct sockaddr*)&client_addr,&client_size);
    if(client_sock<0)
    {
        printf("error while accepting..\n");
        return -1;
    }
    printf("connected to ip:%s and port:%i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    //8.receiving the messages

        if(recv(client_sock,client_message,sizeof(client_message),0)<0)
        {
            printf("error in receiving..\n");
            return -1;
        }
        printf("message received:%s\n",client_message);
        //9.sending the messages
        strcpy(server_message,"hey you succeded..this msg is from server\n");
        if(send(client_sock,server_message,sizeof(server_message),0)<0)
        {
            printf("error in sending..\n");
            return -1;
        }
    
     close(client_sock);
     close(socket_desc);
     return 0;
    

}