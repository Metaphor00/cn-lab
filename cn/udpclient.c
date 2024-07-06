#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(void)
{
    //1.declaring variables
    int socket_desc,client_size;
    struct sockaddr_in server_addr,client_addr;
    char server_message[2000],client_message[2000];
    //2.clearing the memory
    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));
    //3.creating the socket
    socket_desc=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(socket_desc<0)
    {
        printf("error creating the socket...\n");
        return -1;
    }
    printf("socket created successfully...\n");
    //4.setting the port,ip and family
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    
    //6.send msg to the server
    printf("enter the message to send to the server:");
    gets(client_message);
    client_size=sizeof(client_addr);
    if(sendto(socket_desc,client_message,sizeof(client_message),0,(struct sockaddr*)&server_addr,client_size)<0)
    {
        printf("cant send msg to the server...\n");
        return -1;
    }
    printf("msg sent successfully..\n");
    //7.receive msg from the server
    if(recvfrom(socket_desc,server_message,sizeof(server_message),0,(struct sockaddr*)&server_addr,&client_size)<0)
    {
        printf("cant receive from the server...\n");
        return -1;
    }
    printf("message received from the server:%s",server_message);
    close(socket_desc);
    return 0;
}