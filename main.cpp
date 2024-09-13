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
//header de deteccion de discos
#include "disco.h"
//header de deteccion de gpu
#include "gpu.h"
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
	cout << endl;
	//deteccion de discos
	//creamos una instancia de disco
	Disco disco;
	// Llamamos a la función para obtener la información de los discos
	disco.obtenerInfoDiscos();

	cout << endl;
	//deteccion de gpu (con directx)
// Crear una instancia de la clase GPU
	GPU gpuDetector;

	// Obtener la lista de GPUs detectadas
	std::vector<GPUInfo> gpus = gpuDetector.getGPUs();

	// Mostrar la información de las GPUs detectadas
	for (size_t i = 0; i < gpus.size(); i++) {
		std::cout << "GPU #" << i << ": " << gpus[i].name << std::endl;
		std::cout << "Fabricante: " << gpus[i].manufacturer << std::endl;
		std::cout << "-----------------------------" << std::endl;
	}
	return 0;
}