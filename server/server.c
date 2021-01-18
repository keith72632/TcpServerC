#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 2222

int main(int argc, char * argv[]) {
    int hsocket = 0;
    char buffer[1024];
    char clientMessage[200];
    pid_t childPid;
    int newSocket;

    //constuct time out
    //struct timeval tv;
    //tv.tv_sec = 20;
    //tv.tv_usec = 0;
    

    //construct server
    struct sockaddr_in server;
    

    //create socket
    hsocket = socket(AF_INET, SOCK_STREAM, 0);
    if(hsocket == 0){
        printf("SOCKET COULD NOT BE CREATED\n");
        return 1;
    }
    printf("Socket creation sucessful\n");
    memset(&server, '\0', sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    //time out
    //if(setsockopt(hsocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)) < 0){
    //    printf("TIMEOUT\n");
    //    return 1;
    //}

    //bind socket
    if(bind(hsocket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("UNABLE TO BIND SOCK\n");
        return 1;
    }
    printf("Bind sucessful on port: %d\n", PORT);

    listen(hsocket, 3);
    printf("Listening on port: %d\n", PORT);

    while(1) {
        struct sockaddr_in client;
        socklen_t clientLen = sizeof(client);
        

        newSocket = accept(hsocket, (struct sockaddr *)&client, &clientLen);
        if(newSocket < 0){
            perror("newSocket could not be acceped\n");
            exit(1);
        }

        memset(buffer, '\0', sizeof(buffer));
        memset(clientMessage, '\0', sizeof(clientMessage));
        
        printf("Connection accepted from: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        if((childPid = fork() == 0)) {
            close(hsocket);

            while(1) {
                recv(newSocket, buffer, 1024, 0);
                int test = strcmp(buffer, ":exit");
                printf("Test: %d\n\n", test);
                if(strcmp(buffer, ":exit") == 10) {
                    printf("Disconnected from: %s\n", inet_ntoa(client.sin_addr));
                    exit(1);
                } else {
                    printf("Client %s:%d:\n%s\n",inet_ntoa(client.sin_addr), ntohs(client.sin_port), buffer);
                    strncpy(clientMessage, buffer, strlen(buffer));
                    if(send(newSocket, clientMessage, strlen(clientMessage), 0) < 0) {
                        printf("could not send message\n");
                        return 1;
                    }
                    bzero(buffer, sizeof(buffer));
                }
            }
        }
    }     
    close(newSocket);
    return 0;
}
