#include <stdio.h>
#include <string>
//header de deteccion de pantalla
#include "screen.h"
#include "cpu.h"
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

	return 0;
}