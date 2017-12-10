/*
#pragma once

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
*/