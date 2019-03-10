// TelnetClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
//#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "winsock2.h"

DWORD WINAPI ReceiverThread(LPVOID);

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9000);

	connect(client, (SOCKADDR *)&addr, sizeof(addr));

	// Tao luong moi de nhan du lieu tu server
	CreateThread(0, 0, ReceiverThread, &client, 0, 0);

	char buf[256];

	while (true)
	{
		printf("Nhap du lieu: ");
		gets_s(buf, sizeof(buf));

		send(client, buf, strlen(buf), 0);

		if (strncmp(buf, "exit", 4) == 0)
			break;
	}

	closesocket(client);
	WSACleanup();

	return 0;
}


DWORD WINAPI ReceiverThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET *)lpParam;

	char buf[256];
	int ret;

	while (true)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
			break;

		buf[ret] = 0;
		printf("Received: %s\n", buf);
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
