#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define MAX_CLIENTS 100

SOCKET clients[MAX_CLIENTS];
int clientCount = 0;
CRITICAL_SECTION clientsLock;

DWORD WINAPI handleClient(LPVOID clientSocketPtr) {
    SOCKET clientSocket = *(SOCKET*)clientSocketPtr;
    char buffer[1024];
    int bytesReceived;

    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';  // Null-terminate the received data
        printf("Message: %s\n", buffer);  // ✅ Print received message

        EnterCriticalSection(&clientsLock);
        for (int i = 0; i < clientCount; ++i) {
            if (clients[i] != clientSocket) {
                send(clients[i], buffer, bytesReceived, 0);
            }
        }
        LeaveCriticalSection(&clientsLock);
    }

    // Remove client from list
    EnterCriticalSection(&clientsLock);
    for (int i = 0; i < clientCount; ++i) {
        if (clients[i] == clientSocket) {
            clients[i] = clients[--clientCount];  // Replace with last and shrink
            break;
        }
    }
    LeaveCriticalSection(&clientsLock);

    closesocket(clientSocket);
    return 0;
}

int main() {
    WSADATA wsa;
    SOCKET listenSocket, clientSocket;
    struct sockaddr_in serverAddr;

    InitializeCriticalSection(&clientsLock);

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            printf("Accept failed\n");
            continue;
        }

        EnterCriticalSection(&clientsLock);
        if (clientCount < MAX_CLIENTS) {
            clients[clientCount++] = clientSocket;
            CreateThread(NULL, 0, handleClient, &clients[clientCount - 1], 0, NULL);
        } else {
            printf("Max clients reached\n");
            closesocket(clientSocket);
        }
        LeaveCriticalSection(&clientsLock);
    }

    DeleteCriticalSection(&clientsLock);
    closesocket(listenSocket);
    WSACleanup();
    return 0;
}