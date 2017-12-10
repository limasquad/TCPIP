/*
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "Mswsock.lib")

#include <iostream>
#include <WinSock2.h>
#include <MSWSock.h>
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

const char* SERVER_IP = "127.0.0.1";
//const char* SERVER_IP = "0:0:0:0:0:0:0:1"; //"::1";

void ShowFailMessage(char* failedFunctionName)
{
	cout << failedFunctionName << " Failed." << endl;
}

void ErrorQuit(char* message)
{
	LPVOID messageBuffer0;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer0, 0, NULL);

	MessageBox(NULL, (LPCTSTR)messageBuffer0, message, MB_ICONERROR);

	LocalFree(messageBuffer0);

	exit(1);
}

void ErrorDisplay(char* message)
{
	LPVOID messageBuffer0;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer0, 0, NULL);

	printf("[%s] %s", message, (char*)messageBuffer0);

	LocalFree(messageBuffer0);
}

int ReceiveDatum(SOCKET serverSocket, char* buffer, int size, int flags)
{
	int currentReceivedSize0;
	char* currentAddress0;
	int leftSize0;

	currentAddress0 = buffer;
	leftSize0 = size;

	while (leftSize0 > 0)
	{
		currentReceivedSize0 = recv(serverSocket, currentAddress0, leftSize0, flags);

		if (currentReceivedSize0 == SOCKET_ERROR) return SOCKET_ERROR;
		else if (currentReceivedSize0 == 0) break;

		leftSize0 -= currentReceivedSize0;
		currentAddress0 += currentReceivedSize0;
	}

	return (size - leftSize0);
}

int main(int argumentCount, char* argumentVector[])
{
	cout << argumentVector[argumentCount - 1] << endl;

	WSADATA wsaDatum0;
	SOCKET serverSocket0;
	SOCKADDR_IN serverAddress0;

	char sendBuffer[BUFFER_SIZE + 1];
	char receiveBuffer[BUFFER_SIZE + 1];

	int packetBodySize0;

	HANDLE fileHandle0;

	memset(&serverAddress0, 0, sizeof serverSocket0);
	memset(&sendBuffer, 0, sizeof sendBuffer);
	memset(&receiveBuffer, 0, sizeof receiveBuffer);


	if (WSAStartup(0x0202, &wsaDatum0)) return 1;

	serverSocket0 = socket(PF_INET, SOCK_STREAM, 0);

	if (serverSocket0 == INVALID_SOCKET) ErrorQuit("socket()");

	serverAddress0.sin_family = AF_INET;
	serverAddress0.sin_addr.s_addr = inet_addr(SERVER_IP);
	serverAddress0.sin_port = htons(SERVER_PORT);

	if (connect(serverSocket0, (SOCKADDR*)&serverAddress0, sizeof serverAddress0)) ErrorQuit("connect()");



	while (1)
	{
		char fileName[BUFFER_SIZE + 1];
		TRANSMIT_FILE_BUFFERS transmitFileBuffer0;

		transmitFileBuffer0.Head = sendBuffer;
		transmitFileBuffer0.HeadLength = BUFFER_SIZE;
		transmitFileBuffer0.Tail = nullptr;
		transmitFileBuffer0.TailLength = 0;

		cin >> fileName;

		fileHandle0 = CreateFile("SendingText.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, nullptr);
		//CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, fileHandle0);

		ReadFile(fileHandle0, sendBuffer, sizeof sendBuffer, NULL, NULL);

		cout << sendBuffer << endl;

		cout << TransmitFile(serverSocket0, fileHandle0, NULL, NULL, nullptr, &transmitFileBuffer0, TF_DISCONNECT | TF_REUSE_SOCKET) << endl;
		
		if (*sendBuffer == *"/disconnect") break;

		packetBodySize0 = strlen(sendBuffer);

		int sendedSize0 = send(serverSocket0, (char*)&packetBodySize0, sizeof packetBodySize0, 0); //sizeof PACKET_HEADER_SIZE

		if (sendedSize0 == SOCKET_ERROR)
		{
			ErrorDisplay("send()");

			break;
		}

		sendedSize0 = send(serverSocket0, sendBuffer, packetBodySize0, 0);

		if (sendedSize0 == SOCKET_ERROR)
		{
			ErrorDisplay("send()");

			break;
		}

		printf("[TCP Client] Transfer%dBytes\n", sendedSize0);
		
		break;
	}

	closesocket(serverSocket0);

	WSACleanup();

	Sleep(1000);

	return 0;
}
//*/