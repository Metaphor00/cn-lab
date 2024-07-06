#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
//SERVER
int main(void)
{
    //1.declaring variables
    char buffer[100];
    int socket_desc,client_sock,client_size,k=5,m=1,p;
    struct sockaddr_in server_addr,client_addr;
    //2.creating the socket
    socket_desc=socket(AF_INET,SOCK_STREAM,0);
    if(socket_desc<0)
    {
        printf("error while creating the socket...\n");
        return -1;
    }
    printf("socket created successfully..\n");
    //3.setting the port and ip,family
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    //4.binding to the port
    if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        printf("error in binding...\n");
        return -1;
    }
    printf("binding successfull...\n");
    //5.listening for incoming connections
    if(listen(socket_desc,1)<0)
    {
        printf("error in listening...\n");
        return -1;
    }
    printf("listening for incoming connections....\n");
    client_size=sizeof(client_addr);
    //6.accepting incoming connections
    client_sock=accept(socket_desc,(struct sockaddr*)&client_addr,&client_size);
    if(client_sock<0)
    {
        printf("error in accepting..\n");
        return -1;
    }
    printf("successfully connected to ip:%s and port:%i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    //receive frames
    while(k>0)
    {
         if(recv(client_sock,buffer,1024,0)<0)
        {
            printf("error in receiving..\n");
            return -1;
        }
        
            printf("frame %d received\n",m);
            strcpy(buffer,"ACK");
        if(m%2==0)
        {
        //send ack
        if(send(client_sock,buffer,20,0)<0)
        {
            printf("error in sending ack..\n");
            return -1;
        }
        printf("ack %d sent..\n",m);
        }
        else
        {
                strcpy(buffer,"kca");
                printf("acknowledgment %d lost..\n",m);
            
                for(p=1;p<=3;p++)
                {
                    printf("waiting for %d seconds...\n",p);
                    
                }
                printf("retransmitting...\n");
                strcpy(buffer,"ack");
                int y=send(client_sock,buffer,20,0);
                if(y<0)
                {
                    printf("ack not sent");
                    return -1;
                }
                printf("acknowledgment %d sent\n",m);
            
        }

        m++;
        k--;
    }
}