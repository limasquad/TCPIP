/*
#pragma comment(lib, "ws2_32")

#include <iostream>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

enum GLOBAL_VARIABLE
{
	SERVER_PORT = 9000,
	BUFFER_SIZE = 63, // 64 - 1,
	ALLOWED_CLIENT_COUNT = 8,
	PACKET_HEADER_SIZE = 0x00000004, //0000 0000 0000 0000 0000 0000 0000 0100
	//PACKET_BODY_SIZE,
	//PACKET_TRAILER_SIZE,
	//PACKET_SIZE = PACKET_HEADER_SIZE + PACKET_BODY_SIZE + PACKET_TRAILER_SIZE,
};

void ShowFailMessage(char* failedFunctionName)
{
	cout << failedFunctionName << " Failed." << endl;
}

void ErrorQuit(char* message)
{
	LPVOID messageBuffer0;

	FormatMessage(0x00001100, nullptr, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer0, 0, NULL);

	MessageBox(NULL, (LPCTSTR)messageBuffer0, message, MB_ICONERROR);

	LocalFree(messageBuffer0);

	exit(1);
}

void ErrorDisplay(char* message)
{
	LPVOID messageBuffer0;

	FormatMessage(0x00001100, nullptr, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer0, 0, NULL);

	printf("[%s] %s", message, (char*)messageBuffer0);

	LocalFree(messageBuffer0);
}

int ReceiveDatum(SOCKET clientSocket, char* buffer, int size, int flags)
{
	int currentReceivedSize0;
	char* currentAddress0;
	int leftSize0;

	currentAddress0 = buffer;
	leftSize0 = size;

	while (leftSize0 > 0)
	{
		currentReceivedSize0 = recv(clientSocket, currentAddress0, leftSize0, flags);

		if (currentReceivedSize0 == SOCKET_ERROR) return SOCKET_ERROR;
		else if (currentReceivedSize0 == 0) break;

		leftSize0 -= currentReceivedSize0;
		currentAddress0 += currentReceivedSize0;
	}

	return (size - leftSize0);
}

int ReceiveCount(SOCKET clientSocket, char* receiveBuffer, int receiveBufferSize, int flags)
{
	int currentReceivedSize0;
	char* currentReceiveBufferAddress0 = receiveBuffer;
	int currentLeftReceiveBufferSize0 = receiveBufferSize;

	while (currentLeftReceiveBufferSize0 > 0)
	{
		currentReceivedSize0 = recv(clientSocket, currentReceiveBufferAddress0, currentLeftReceiveBufferSize0, flags);

		if (currentReceivedSize0 == SOCKET_ERROR) return SOCKET_ERROR;
		else if (currentReceivedSize0 == 0) break;

		currentLeftReceiveBufferSize0 -= currentReceivedSize0;
		currentReceiveBufferAddress0 += currentReceivedSize0;
	}

	return (receiveBufferSize - currentLeftReceiveBufferSize0);
}


int main(int argumentCount, char* argumentVector[])
{
	cout << argumentVector[argumentCount - 1] << endl;

	WSADATA wsaDatum0;
	SOCKET serverSocket0;
	SOCKADDR_IN serverAddress0;

	SOCKET clientSocket[ALLOWED_CLIENT_COUNT];
	SOCKADDR_IN clientAddress[ALLOWED_CLIENT_COUNT];

	char sendBuffer[BUFFER_SIZE + 1];
	char receiveBuffer[BUFFER_SIZE + 1];

	int packetBodySize0;

	memset(&serverAddress0, 0, sizeof serverSocket0);
	memset(&clientSocket, 0, sizeof clientSocket);

	memset(&sendBuffer, 0, sizeof sendBuffer);
	memset(&receiveBuffer, 0, sizeof receiveBuffer);


	if (WSAStartup(0x0202, &wsaDatum0)) return 1;

	serverSocket0 = socket(PF_INET, SOCK_STREAM, 0);

	if (serverSocket0 == INVALID_SOCKET) ErrorQuit("socket()");

	serverAddress0.sin_family = AF_INET;
	serverAddress0.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress0.sin_port = htons(SERVER_PORT);

	if (bind(serverSocket0, (SOCKADDR*)&serverAddress0, sizeof serverAddress0)) ErrorQuit("bind()"); //ShowFailMessage("bind()");

	if (listen(serverSocket0, SOMAXCONN)) ErrorQuit("listen()");



	while (1)
	{
		//if()
		{
			int clientAddressSize0 = sizeof clientAddress[0];
			clientSocket[0] = accept(serverSocket0, (SOCKADDR*)&clientAddress[0], &clientAddressSize0);

			if (clientSocket[0] == INVALID_SOCKET)
			{
				ErrorDisplay("accept()");
				break;
			}

			printf("\n[TCP_Server] ClientConnection : IP_Address = %s, PortNumber = %d\n", inet_ntoa(clientAddress[0].sin_addr), ntohs(clientAddress[0].sin_port));
		}

		while (1)
		{
			int receivedSize0 = ReceiveCount(clientSocket[0], (char*)&packetBodySize0, sizeof packetBodySize0, 0);

			if (receivedSize0 == SOCKET_ERROR)
			{
				ErrorDisplay("recv()");

				break;
			}
			else if (receivedSize0 == 0) break;

			receivedSize0 = ReceiveCount(clientSocket[0], receiveBuffer, packetBodySize0, 0);

			if (receivedSize0 == SOCKET_ERROR)
			{
				ErrorDisplay("recv()");

				break;
			}
			else if (receivedSize0 == 0) break;

			receiveBuffer[receivedSize0] = '\0';

			printf("[TCP %s : %d] %s\n", inet_ntoa(clientAddress[0].sin_addr), ntohs(clientAddress[0].sin_port), receiveBuffer);
		}


		closesocket(clientSocket[0]);

		printf("[TCP_Server] ClientDisconnection : IP_Address = %s, PortNumber = %d\n", inet_ntoa(clientAddress[0].sin_addr), ntohs(clientAddress[0].sin_port));
	}

	closesocket(serverSocket0);

	WSACleanup();

	return 0;
}
//*/