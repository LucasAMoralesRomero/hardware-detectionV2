#include "ram.h"

//CONSTRUCTOR DE LA CLASE
ram::ram()
{
	infoMemory.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&infoMemory);
}
//OBTENEMOS LA CANTIDAD DE MEMORIA RAM EN GB
double ram::getRam()
{
	//convertimos los bytes de memoria en GB
	return static_cast<double>(infoMemory.ullTotalPhys) / (1024 * 1024 * 1024);
}
