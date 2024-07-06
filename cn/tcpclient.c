#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(void)
{
    //1.declaring variables
    int socket_desc;
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
    //5.connect with the server
    if(connect(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        printf("error connecting..\n");
        return -1;
    }
    printf("connected with server successfully...\n");
    //6.getting the message and sending
    
        printf("Enter the message to send:\n");
        gets(client_message);
        if(send(socket_desc,client_message,strlen(client_message),0)<0)
        {
            printf("error in sending..\n");
            return -1;
        }
        //7.receiving message from the server
        if(recv(socket_desc,server_message,sizeof(server_message),0)<0)
        {
            printf("not receiving..\n");
            return -1;
        }
        printf("message from server:%s\n",server_message);
    
    close(socket_desc);
    return 0;
}