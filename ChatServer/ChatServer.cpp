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
void RemoveClient(SOCKET);
DWORD WINAPI ClientThread(LPVOID lpParam);
void Disconnect(SOCKET s);

SOCKET listClients[64];
int no_Client = 0;

int main(int argc, char * argv[]) {
	WSADATA wsa;
	int ret = WSAStartup(0x0202, &wsa);
	if (ret == 0) {
		cout << "Initialize WSA" << endl;
	}
	else {
		cout << "Error while start up ";
		return -2;
	}
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	int port = atoi(argv[1]);
	if (port < 0) {
		cout << "Wrong port!!";
		return -1;
	}
	SOCKADDR_IN addrin;
	addrin.sin_family = AF_INET;
	addrin.sin_addr.s_addr = htonl(INADDR_ANY);
	addrin.sin_port = htons(port);

	bind(server, (SOCKADDR*)&addrin, sizeof(addrin));
	listen(server, 5);
	cout << "Listening at port " << port << endl;
	while (true) {
		SOCKET client = accept(server, NULL, NULL);
		cout << "Accept new client: " << client << endl;
		listClients[no_Client] = client;
		no_Client++;
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}
	WSACleanup();
}

DWORD WINAPI ClientThread(LPVOID lpParam) {
	SOCKET client = *(SOCKET*)lpParam;
	char buf[256];
	int ret;
	char id[64] = "";
	char name[64];
	while (true) {
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0) {
			Disconnect(client);
			break;
		}
		buf[ret] = 0;
		if (strcmp(id, "client_id:")) {
			sscanf(buf, "%s%s", id, name);
		}
		else {
			cout << "Receive: " << buf << " from " << buf << endl;
			for (int i = 0; i < no_Client; i++)
			{
				if (listClients[i] == client) continue;
				// get system current time string 
				time_t rawtime;
				struct tm * timeinfo;
				char timestring[80];
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				strftime(timestring, sizeof(timestring), "[%d-%m-%Y %H:%M:%S] ", timeinfo);
				// 
				send(listClients[i], timestring, strlen(timestring), 0);
				send(listClients[i], name, strlen(name), 0);
				send(listClients[i], ": ", 2, 0);
				send(listClients[i], buf, strlen(buf), 0);
			}
		}
	}
	return 0;
}

void Disconnect(SOCKET s) {
	cout <<"Client "<< s << " has disconnected." << endl;
	int i = 0;
	for (; i < no_Client; i++) {
		if (listClients[i] == s)
			break;
	}
	if (i < no_Client - 1) {
		listClients[i] = listClients[no_Client - 1];
	}
	no_Client--;
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
