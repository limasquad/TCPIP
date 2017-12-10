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
	ALLOWED_CLIENT_COUNT = 8
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

int ReceiveAhead(SOCKET clientSocket, char* receivedByte)
{
	__declspec(thread) static char buffer[1024];
	__declspec(thread) static int currentByteCount0 = 0;
	__declspec(thread) static char* currentBufferAddress0;

	if (currentByteCount0 == 0 || currentByteCount0 == SOCKET_ERROR)
	{
		currentByteCount0 = recv(clientSocket, buffer, sizeof buffer, 0);

		if (currentByteCount0 == SOCKET_ERROR) return SOCKET_ERROR;
		else if (currentByteCount0 == 0) return 0;

		currentBufferAddress0 = buffer;
	}
	
	--currentByteCount0;
	*receivedByte = *currentBufferAddress0++;

	return 1;
}

int ReceiveLine(SOCKET clientSocket, char* receiveBuffer, int maxLength)
{
	int accumulatedByteCount0;
	int currentByteCount0;
	char receivedByte0;
	char* currentBufferAddress0;
	
	currentBufferAddress0 = receiveBuffer;

	for (accumulatedByteCount0 = 1; accumulatedByteCount0 < maxLength; ++accumulatedByteCount0)
	{
		currentByteCount0 = ReceiveAhead(clientSocket, &receivedByte0);

		if (currentByteCount0 == 1)
		{
			*currentBufferAddress0++ = receivedByte0;

			if (receivedByte0 == '\n') break;
		}
		else if (currentByteCount0 == 0)
		{
			*currentBufferAddress0 = 0;
			
			return accumulatedByteCount0 - 1;
		}
		else return SOCKET_ERROR;
	}

	*currentBufferAddress0 = 0;

	return accumulatedByteCount0;
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
			int clientAddressSize0;

			clientAddressSize0 = sizeof clientAddress[0];
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
			int receivedSize0 = ReceiveLine(clientSocket[0], receiveBuffer, BUFFER_SIZE + 1);

			if (receivedSize0 == SOCKET_ERROR)
			{
				ErrorDisplay("recv()");
				break;
			}
			else if (receivedSize0 == 0) continue;

			printf("[TCP %s : %d] %s\n", inet_ntoa(clientAddress[0].sin_addr), ntohs(clientAddress[0].sin_port), receiveBuffer);

			{
				int currentBufferSize0;
				int sendedSize0;

				//scanf("%s", sendBuffer);
				cin >> sendBuffer;

				if (*sendBuffer == *"/disconnect") break;

				currentBufferSize0 = strlen(sendBuffer);
				sendBuffer[currentBufferSize0++] = '\n';

				sendedSize0 = send(clientSocket[0], sendBuffer, currentBufferSize0, 0);

				if (sendedSize0 == SOCKET_ERROR)
				{
					ErrorDisplay("send()");

					break;
				}

				printf("[TCP Client] Transfer%dBytes\n", sendedSize0);
			}
		}

		closesocket(clientSocket[0]);

		printf("[TCP_Server] ClientDisconnection : IP_Address = %s, PortNumber = %d\n", inet_ntoa(clientAddress[0].sin_addr), ntohs(clientAddress[0].sin_port));
	}

	closesocket(serverSocket0);

	WSACleanup();

	return 0;
}
//*/