#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
//CLIENT
int main(void)
{
    //1.declaring variables
    char buffer[100];
    int socket_desc,client_size,k=5,m=1,p;
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
   
    //4.connect to the server
    if(connect(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        printf("error in connecting..\n");
        return -1;
    }
    printf("connected successfully to the receiver...\n");
    while(k>0)
    {
        //sending frames only if m%2==0 else simulate packet loss and retransimit after 3 seconds
        if(m<=5)
        {
            printf("sending frames..\n");
            strcpy(buffer,"FRAME");
            if(m%2==0)
            {
                int y=send(socket_desc,buffer,20,0);
                if(y<0)
                {
                    printf("frame not sent");
                    return -1;
                }
                printf("frame %d sent\n",m);
            }
            else
            {
                strcpy(buffer,"error");
                printf("frame %d lost...\n",m);
                for(p=1;p<=3;p++)
                {
                    printf("waiting for %d seconds...\n",p);
                    
                }
                printf("retransmitting...\n");
                strcpy(buffer,"frame");
                int y=send(socket_desc,buffer,20,0);
                if(y<0)
                {
                    printf("frame not sent");
                    return -1;
                }
                printf("frame %d sent\n",m);
            }
        }
        //receive ack 
        if(recv(socket_desc,buffer,1024,0)<0)
        {
            printf("not receiving ack..\n");
            return -1;
        }
        printf("acknowledgment %d received from sender.\n",m);
        m++;
        k--;
        

    }

    close(socket_desc);
}