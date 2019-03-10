// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <stdio.h>

int main(void)
{
	const char hhmmss[] = "10:32:54";
	int hh, mm, ss;
	char test[10] = "12:fggg";
	char txt1[10]="";
	char txt2[10] = "";
	int n;
	sscanf(test, "%i:%s", &n, txt2);
	printf("%i\n", n);
	printf("%s\n", txt2);
	if (sscanf(hhmmss, "%i:%i:%i", &hh, &mm, &ss) != 3)
		printf("Failed to scan 3 values from '%s'\n", hhmmss);
	else
		printf("From <<%s>> hh = %d, mm = %d, ss = %d\n", hhmmss, hh, mm, ss);
	return 0;
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
