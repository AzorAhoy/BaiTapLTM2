// ChatServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
//#include "stdafx.h"
#include "winsock2.h"
#include "string.h"
#include "time.h"
#include <ctime>
using namespace std;

DWORD WINAPI ClientThread(LPVOID);

SOCKET Clients[64];
int numClients;
char * ids[64];

int main(int argc, char * argv[]) {
	WSADATA wsa;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int port = 9000;
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	bind(server, (SOCKADDR*)&addr, sizeof(addr));
	listen(server, 5);

	numClients = 0;

	cout << "Listening at port " << port << endl;
	while (true) {
		SOCKET client = accept(server, NULL, NULL);
		printf("Accept new client: %d\n", client);
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}
	return 0;
}

DWORD WINAPI ClientThread(LPVOID lpParam) {
	SOCKET client = *(SOCKET*)lpParam;

	char buf[256];
	char sendBuf[256];
	int ret;

	char cmd[64];
	char id[64];
	char tmp[64];

	char targetID[64];
	char errorMsg[] = "Syntax Error. Please try again.\n";

	while (true) {
		ret = recv(client, buf, sizeof(buf), 0);

		buf[ret] = 0;
		printf("Received: %s\n", buf);
		//Kiem tra cu phap: "client_id:[id]"
		ret = sscanf(buf, "%s %s %s", cmd, id, tmp);
		if (ret == 2)
		{
			if (strcmp(cmd,"client_id:") == 0)
			{
				char okMsg[] = "CorrectSyntax. Type your message\n";
				send(client, okMsg, strlen(okMsg), 0);
				//Luu client dang nhap thanh cong vao mang
				Clients[numClients] = client;
				ids[numClients] = id;
				numClients++;

				break;
			}
			else
			{
				send(client, errorMsg, strlen(errorMsg), 0);
			}
		}
		else
		{
			send(client, errorMsg, strlen(errorMsg), 0);
		}
	}

	while (true)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		buf[ret] = 0;
		printf("Received: %s\n", targetID);
		if (ret == 1)
		{
			if (strcmp(targetID, "all") == 0)
			{
				sprintf(sendBuf, "%s: %s", id, buf + strlen(targetID) + 1);
				for (int i = 0; i < numClients; i++)
				{
					if (Clients[i] != client)
					{
						send(Clients[i], sendBuf, strlen(sendBuf), 0);
					}
				}
			}
			else
			{
				sprintf(sendBuf, "%s: %s", id, buf + strlen(targetID) + 1);
				for (int i = 0; i < numClients; i++)
				{
					if (strcmp(ids[i], targetID) == 0)
					{
						send(Clients[i],sendBuf,strlen(sendBuf),0);
					}
				}
			}
		}
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
