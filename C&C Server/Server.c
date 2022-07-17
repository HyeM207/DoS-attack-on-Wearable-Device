#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
#define BUFSIZE 1024
#define PORT 8888

void ErrorHandling(char* message);

int main(int argc, char** argv) {

    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    SOCKADDR_IN serverAddress, clientAddr;

    int msgLen;
    char message[BUFSIZE] = { 0, };
    char SendMsg[BUFSIZE] = "192.168.253.130:7777"; 
    int clientSize;

    // 0. WSAStartup()
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        ErrorHandling("WSAStartup() Error!");
    }


    // 1. socket()
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        ErrorHandling("socket() Error!");
    }


    // + 소켓 설정
    int option = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option));


    // 2. 서버 주소 초기화 
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);  // INADDR_ANY : 서버 ip 주소 자동 할당 
    serverAddress.sin_port = htons(PORT);

    //3.bind()
    if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        ErrorHandling("bind() Error!");
    }

    // 4. listen()
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        ErrorHandling("listen() Error!");
    }

    clientSize = sizeof(clientAddr);
    printf("listening...");

    while (1) {

        // 5. accept()
        clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientSize);

        if (clientSocket == INVALID_SOCKET) {
            ErrorHandling("accept() Error!");
        }
        else {
            printf("Accept client \n");
        }
        
        // recv()
        while (1) {

            msgLen = recv(clientSocket, message, BUFSIZE, 0);

            if (msgLen == -1) {
                printf("disconnect client \n");
                break;
            }
           
            // End socket
            if (message == "end")
            {
                printf("End socket programming\n");
                break;
            }

            // recv ()
            if (msgLen != 0) {
                printf("Recv message : %s , length : %d \n", message, msgLen);
                memset(message, 0, BUFSIZE); // 받은 문자열 담은 배열 초기화 
            }

            // send ()
            char response ;
            printf("Do you want to send Message to Client(Tizen)?  (y/n) : ");
            scanf("%c", &response);
            getchar();
            printf("response : %c\n", response);

            if (response == 'y') {
                send(clientSocket, SendMsg, msgLen, 0);
                printf("Send message : %s , length : %d \n", SendMsg, msgLen);
                break;
            }
            else {
                if (response == 'n') {
                    printf("Not to Send\n");
                }
                else {
                    printf("Wrong Input (Please enter 'y' or 'n')\n");
                }
            }

        }
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }

}

void ErrorHandling(char* message) {
    WSACleanup();
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
