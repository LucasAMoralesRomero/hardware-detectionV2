#pragma once
#include <windows.h>
#include <dxgi.h>
#include <string>
#include <vector>

// Estructura que almacena la información de una GPU
struct GPUInfo {
	std::string name;
	std::string manufacturer; // Fabricante de la GPU (AMD, NVIDIA, Intel)
};

// Clase para detectar GPUs usando DXGI
class GPU {
public:
	GPU();
	~GPU();

	// Método para obtener la lista de GPUs detectadas
	std::vector<GPUInfo> getGPUs() const;

private:
	std::vector<GPUInfo> gpus; // Lista de GPUs detectadas

	// Método auxiliar para detectar GPUs
	void detectGPUs();

	// Método auxiliar para determinar el fabricante
	std::string getManufacturer(unsigned int vendorId);
};