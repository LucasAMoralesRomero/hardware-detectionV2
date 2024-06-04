#pragma once
#include <iostream>
#include<Windows.h>
#include <string>

using namespace std;

class screen
{
public:
	//funcion que devuelve el tamaño de la pantalla
	string getScreen();

private:
	//integer para guardar el ancho de pantalla
	int width = 0;
	//integer para guardar el alto de pantalla
	int height = 0;

};