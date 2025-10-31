#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define SERVER_IP "127.0.0.1"

SOCKET s;
char name[50];

DWORD WINAPI sendMessage(LPVOID lpParam) {
    char message[1024], fullMessage[1074];

    printf("Enter your chat name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    while (1) {
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;

        snprintf(fullMessage, sizeof(fullMessage), "%s : %s", name, message);
        if (send(s, fullMessage, strlen(fullMessage), 0) == SOCKET_ERROR) {
            printf("Send failed\n");
            break;
        }

        if (strcmp(message, "quit") == 0) {
            break;
        }
    }

    return 0;
}

DWORD WINAPI receiveMessage(LPVOID lpParam) {
    char buffer[1024];
    int recvLen;

    while ((recvLen = recv(s, buffer, sizeof(buffer), 0)) > 0) {
        buffer[recvLen] = '\0';
        printf("%s\n", buffer);
    }

    printf("Disconnected from server\n");
    closesocket(s);
    WSACleanup();
    exit(0);
    return 0;
}

int main() {
    WSADATA wsa;
    struct sockaddr_in serverAddr;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(s, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Connection failed\n");
        closesocket(s);
        WSACleanup();
        return 1;
    }

    printf("Connected to server\n");

    CreateThread(NULL, 0, sendMessage, NULL, 0, NULL);
    CreateThread(NULL, 0, receiveMessage, NULL, 0, NULL);

    while (1) Sleep(1000); // Keep main thread alive

    return 0;
}