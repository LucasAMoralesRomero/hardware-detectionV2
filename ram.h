#pragma once
#include <Windows.h>

class ram {
public:
	//Funcion constructora
	ram();

	//Funcion para obtener la cantidad de memoria ram en gb
	double getRam();
private:
	//definimos un objeto para obtener la cantidad de memoria
	MEMORYSTATUSEX infoMemory;
};