#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>

#define PORT 80

const char szHost[] = "www.google.com";

#define EXIT(msg) MessageBoxA(nullptr, msg, "Error!", MB_OK); ExitProcess(EXIT_FAILURE)

int main()
{
	// Initalize WINSOCK
	WSAData wsaData;
	WORD dllVersion = MAKEWORD(2, 1);
	if (WSAStartup(dllVersion, &wsaData) != 0)
	{
		EXIT("Failed to initialize WINSOCK");
	}

	// Create Socket
	// AF_INET = we want to use the internet
	// SOCK_STREAM = we want to use tcp
	// 0 = we don't want to follow any specific protocol
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mySocket < 0)
	{
		EXIT("Failed to create socket");
	}

	// Get the server info
	HOSTENT* host = gethostbyname(szHost);
	if (host == nullptr)
	{
		EXIT("Failed to locate server by name");
	}

	// Define Server Info
	SOCKADDR_IN sin;
	ZeroMemory(&sin, sizeof(SOCKADDR_IN));
	sin.sin_port = htons(PORT);
	sin.sin_family = AF_INET;
	memcpy(&sin.sin_addr.S_un.S_addr, host->h_addr_list[0], sizeof(sin.sin_addr.S_un.S_addr));

	// Connect to server
	if (connect(mySocket, (const sockaddr*)&sin, sizeof(sin)) != 0)
	{
		EXIT("Failed to connect to server");
	}

	const char szMsg[] = "HEAD / HTTP/1.0\r\n\r\n";
	if (!send(mySocket, szMsg, strlen(szMsg), 0))
	{
		EXIT("Failed to send message");
	}

	char szBuffer[4096];
	char szTemp[4096];

	while (recv(mySocket, szTemp, 4096, 0))
	{
		strcat(szBuffer, szTemp);
	}

	printf("%s\n", szBuffer);

	closesocket(mySocket);
	getchar();

	ExitProcess(EXIT_SUCCESS);
}