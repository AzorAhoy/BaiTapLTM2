// TelnetServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
//#include "stdafx.h"
#include "winsock2.h"
#include <fstream>
#include <string>

using namespace std;

DWORD WINAPI ClientThread(LPVOID);
void Disconnect(SOCKET);
void openFile();

CRITICAL_SECTION cs;
SOCKET listClients[64];
char username[64][64];
char password[64][64];
int account = 0;

int no_client = 0;

int main(int argc, char * argv[]) {

	WSADATA wsa;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsa);

	int port = atoi(argv[1]);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);

	while (true) {
		SOCKET client = accept(listener, NULL, NULL);
		printf("Accept client: %d\n", client);
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}

	return 0;
}

DWORD WINAPI ClientThread(LPVOID s) {
	SOCKET client = *(SOCKET*)s;
	char buf[256];
	int ret;
	char fileBuf[256];
	char cmdBuf[256];

	while (true) {
		ret = recv(client, buf, sizeof(buf), 0);
		buf[ret] = 0;
		printf("Received: %s\n", buf);

		int found = 0;
		FILE *f = fopen("users.txt", "r");
		while (fgets(fileBuf, sizeof(fileBuf), f))
		{
			if (strcmp(buf, fileBuf) == 0)
			{
				found = 1;
				break;
			}
		}
		fclose(f);

		if (found == 1)
		{
			char msg[] = "Dang nhap thanh cong. Hay nhap lenh.\n";
			send(client, msg, strlen(msg), 0);
		}
		else
		{
			char msg[] = "Dang nhap that bai. Hay thu lai.\n";
			send(client, msg, strlen(msg), 0);
		}
	}

	while (true) {
		//char buf[256];
		ret = recv(client, buf, sizeof(buf), 0);

		buf[ret] = 0;
		if (buf[ret - 1] == '\n')
		{
			buf[ret - 1] = 0;
		}
		printf("Received: %s\n", buf);
		printf(cmdBuf, "%s > C:\\Users\\Administrator\\source\\repos\\BaiTapLTM2\\TelnetServer\\out.txt", buf);
		sprintf(cmdBuf, "%s > out.txt", buf);
		system(cmdBuf);

		FILE *f;
		f = fopen("out.txt", "r");
		while (fgets(fileBuf, sizeof(fileBuf), f))
		{
			send(client, fileBuf, strlen(fileBuf), 0);
		}
		fclose(f);
		//LeaveCriticalSection(&cs);
	}
	//return 0;
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
