#include <stdio.h>
#include <string>
//header de deteccion de pantalla
#include "screen.h"
using namespace std;

int main(int argc, char *argv[])
{
	screen * pantalla = new screen();

	cout << pantalla->getScreen();

	return 0;
}