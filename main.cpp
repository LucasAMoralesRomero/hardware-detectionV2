#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
//header de deteccion de pantalla
#include "screen.h"
//header de deteccion de CPU
#include "cpu.h"
//header de detecion de memoria ram
#include "ram.h"
using namespace std;

int main(int argc, char *argv[])
{
	screen * pantalla = new screen();

	cout << pantalla->getScreen() << endl;
	 //creamos una instancia de cpu
	cpu cpu;
	std::string name;
	int cores = 0;
	int logicalProcessors = 0;
	double maxClockSpeed = 0.0;

	if (cpu.getCPUInfo(name, cores, logicalProcessors, maxClockSpeed)) {
		std::cout << "Nombre del CPU: " << name << std::endl;
		std::cout << "Número de Núcleos Físicos: " << cores << std::endl;
		std::cout << "Número de Procesadores Lógicos: " << logicalProcessors << std::endl;
		std::cout << "Velocidad Máxima del Reloj: " << maxClockSpeed << " GHz" << std::endl;
	}
	else {
		std::cerr << "No se pudo obtener la información del CPU." << std::endl;
	}
	ram * memoriaRam = new ram;

	cout << "Memoria RAM:" << fixed << setprecision(1) <<memoriaRam->getRam() << " Gb." << endl;

	return 0;
}