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
    char message[1024];
    char incomingMessage[1024];
    int ret = 0;
    struct sockaddr_in server;

    hsocket = socket(AF_INET, SOCK_STREAM, 0);

    if(hsocket < 0) {
        printf("SOCKET COULD NOT BE CREATED\n");
        exit(1);
    }
    printf("Socket created\n");

    memset(&server, '\0', sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = connect(hsocket, (struct sockaddr *)&server, sizeof(server));
    if(ret < 0) {
        printf("CONNECTION UNSUCESSFUL\n");
        exit(1);
    }
    printf("Socket connected to port: %d\n", PORT);    

    while(1){
        printf("Client:\t");
        fgets(message, 1024, stdin);
        send(hsocket, message, strlen(message), 0);
        int test = strcmp(message, ":exit");
        printf("Test: %d\n\n", test);
        if(strcmp(message, ":exit") == 10) {
            close(hsocket);
            printf("Disconnecting from the server...\n");
            exit(1);
        }
        if(recv(hsocket, incomingMessage, 1024, 0) < 0) {
            printf("Error, disonecting from server...\n");
        } else {
            printf("Sever: %s\n", incomingMessage);
        }
    }

   return 0; 
}