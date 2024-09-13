#include "so.h"
#include <Windows.h>
#include <cstdio>
#include <iostream>
using namespace std;

void so::detectSO() {
	//accedemos a la estructura compratida de datos de windows
	auto sharedUserData = (BYTE*)0x7FFE0000;
	//vemos la mayor version de windows
	ULONG majorVersion = *(ULONG*)(sharedUserData + 0x26c);
	//vemos la version menor de windows
	ULONG minorVersion = *(ULONG*)(sharedUserData + 0x270);
	//vemos la version de compilacion de windows
	ULONG buildNumber = *(ULONG*)(sharedUserData + 0x260);
	cout << "Version: " << majorVersion << "." << minorVersion << "." << buildNumber << endl;
}