#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

void main() {

    //----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR)
        printf("Error at WSAStartup()\n");

    //----------------------
    // Create a SOCKET for listening for
    // incoming connection requests.
    SOCKET ListenSocket;
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(27015);

    if (bind( ListenSocket, 
              (SOCKADDR*) &service, 
              sizeof(service)) == SOCKET_ERROR)
    {
        printf("bind() failed.\n");
        closesocket(ListenSocket);
        return;
    }

    //----------------------
    // Listen for incoming connection requests.
    // on the created socket
    if (listen( ListenSocket, 1 ) == SOCKET_ERROR)
        printf("Error listening on socket.\n");

    //----------------------
    // Create a SOCKET for accepting incoming requests.
    SOCKET AcceptSocket;
    printf("Waiting for client to connect...\n");

    //----------------------
    // Accept the connection.
    DWORD dwRet;
    AcceptSocket = SOCKET_ERROR;
    while (1)
    {
        AcceptSocket = accept( ListenSocket, NULL, NULL );
        if (SOCKET_ERROR != AcceptSocket)
        {
            PlaySound("SystemExit",NULL, SND_ALIAS);
            printf("Exiting\n");
            //LockWorkStation();
            break;
        }

        // OK need to look for the object to know when to exit
        // Wait for object is bad.
        //dwRet = WaitForSingleObject(hServerStopEvent,  //const HANDLE* pHandles,
        //                           0 );   // Test and leave
        //if (WAIT_OBJECT_0 == dwRet)
        //{
        //    //Event Signaled -- Service told to shut 
        //    break;
        //}
    }

    WSACleanup();
    return;
}

