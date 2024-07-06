#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(void)
{
    //1.declaring variables
    FILE *fp;
    int n,s;
    int socket_desc;
    struct sockaddr_in server_addr,client_addr;
    char name[100],rcvmsg[100],rcvg[100],fname[100];
    //2.clearing the memory
   
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
    printf("enter file name:");
    scanf("%s",name);
    printf("enter new file name:");
    scanf("%s",fname);

    fp=fopen(fname,"w");
    send(socket_desc,name,sizeof(name),0);

    while(1)
    {
        s=recv(socket_desc,rcvg,100,0);
        rcvg[s]='\0';
        if(strcmp(rcvg,"error")==0)
        {
            printf("file is not available");
            exit(1);
        }
        if(strcmp(rcvg,"completed")==0)
        {
            printf("\nfile is transferred...\n");
            fclose(fp);
            break;
        }
        else{
            printf("the file contents are:");
            fputs(rcvg,stdout);
            fprintf(fp,"%s",rcvg);
        }
    }
close(socket_desc);
return 0;


}