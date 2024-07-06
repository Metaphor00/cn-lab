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
    //5.binding the port
    if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        printf("error binding to the port..\n");
        return -1;
    }
    printf("binding done successfully\n");
    //6.receive from the client
    client_size=sizeof(client_addr);
    if(recvfrom(socket_desc,client_message,sizeof(client_message),0,(struct sockaddr*)&client_addr,&client_size)<0)
    {
        printf("error receiving the message...\n");
        return -1;
    }
    printf("message received from the client:%s\n",client_message);
    strcpy(server_message,"yay you succeeded in sending a message...\n");
    if(sendto(socket_desc,server_message,sizeof(server_message),0,(struct sockaddr*)&client_addr,client_size)<0)
    {
        printf("error sending message from the server...\n");
        return -1;
    }
    printf("message sent succcessfully to the client..\n");
    close(socket_desc);
    close(client_sock);

    return 0;
}