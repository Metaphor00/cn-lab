#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/times.h>
#include <sys/types.h>
#include <netdb.h>

#define MAX 80
#define PORT 8080

struct timeval timeout;

void func(int sockfd, int nf, int ws) {
    char buff[MAX];
    int ack, i = 0, n, k, w1 = 0, w2 = ws - 1, j, flag = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0)
        perror("setsockopt(SO_RCVTIMEO) failed");

    // Initial sending of frames within the window
    for (i = 0; i < nf && i <= w2; i++) {
        bzero(buff, sizeof(buff));
        snprintf(buff, sizeof(buff), "%d", i);
        k = send(sockfd, buff, sizeof(buff), 0);
        printf("frame %d sent\n", i);
    }

    while (1) {
        // Sending new frames within the window if any
        if (w2 - w1 != ws - 1 && flag == 0 && i < nf) {
            bzero(buff, sizeof(buff));
            snprintf(buff, sizeof(buff), "%d", i);
            k = send(sockfd, buff, sizeof(buff), 0);
            printf("frame %d sent\n", i);
            w2++;
            i++;
        }
        //receiving ack
        flag = 0;
        bzero(buff, sizeof(buff));
        n = recv(sockfd, buff, MAX, 0);
        ack = atoi(buff);

        if (n > 0) {//ack present
            if (ack + 1 == nf) {//aack for last frame
                printf("Acknowledgment received: %d\nExit\n", ack);
                bzero(buff, sizeof(buff));
                strcpy(buff, "Exit");
                k = send(sockfd, buff, sizeof(buff), 0);
                break;
            }

            if (ack == w1) {//ack for first frame
                w1++;//slide the window
                printf("Acknowledgment received: %d\n", ack);
            }
        } else {//ack not received
            printf("acknowledgment not received for %d\n resending frames\n", w1);
            for (j = w1; j < nf && j <= w2; j++) {
                bzero(buff, sizeof(buff));
                snprintf(buff, sizeof(buff), "%d", j);
                k = send(sockfd, buff, sizeof(buff), 0);
                printf("frame %d sent\n", j);
            }
            flag = 1;
        }
    }
}

int main(void) {
    int socket_desc, f, w;
    struct sockaddr_in server_addr;

    // 1. Creating the socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        printf("error creating the socket...\n");
        return -1;
    }
    printf("socket created successfully...\n");
    
    bzero(&server_addr, sizeof(server_addr));

    // 2. Setting the IP address, port, and family
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    // 3. Connect with the server
    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("error connecting...\n");
        return -1;
    }
    printf("connected with server successfully...\n");

    // 4. User input for the number of frames and window size
    printf("enter the number of frames:\n");
    scanf("%d", &f);
    printf("enter the window size:\n");
    scanf("%d", &w);

    // 5. Running the Go-Back-N protocol
    func(socket_desc, f, w);

    close(socket_desc);
    return 0;
}
