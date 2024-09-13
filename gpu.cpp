#include "gpu.h"
#include <iostream>

#pragma comment(lib, "dxgi.lib")

GPU::GPU() {
	detectGPUs(); // Detectar GPUs al crear la instancia
}

GPU::~GPU() {
	// Destructor por defecto
}

// Implementación del método para obtener la lista de GPUs detectadas
std::vector<GPUInfo> GPU::getGPUs() const {
	return gpus;
}

// Método que realiza la detección de GPUs
void GPU::detectGPUs() {
	IDXGIFactory* pFactory = nullptr;

	// Crear una fábrica DXGI
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (FAILED(hr)) {
		std::cerr << "No se pudo crear la fábrica DXGI." << std::endl;
		return;
	}

	IDXGIAdapter* pAdapter = nullptr;
	int adapterIndex = 0;

	while (pFactory->EnumAdapters(adapterIndex, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
		DXGI_ADAPTER_DESC adapterDesc;
		pAdapter->GetDesc(&adapterDesc);

		// Convertir el nombre del adaptador a un string legible
		std::wstring ws(adapterDesc.Description);
		std::string adapterName(ws.begin(), ws.end());

		// Llenar la estructura de GPUInfo con los datos del adaptador
		GPUInfo gpu;
		gpu.name = adapterName;
		gpu.manufacturer = getManufacturer(adapterDesc.VendorId); // Determinar el fabricante

		// Agregar la GPU detectada a la lista
		gpus.push_back(gpu);

		// Liberar el adaptador
		pAdapter->Release();
		adapterIndex++;
	}

	pFactory->Release();
}

// Método para determinar el fabricante según el Vendor ID
std::string GPU::getManufacturer(unsigned int vendorId) {
	switch (vendorId) {
	case 4318: // NVIDIA
		return "NVIDIA";
	case 4098: // AMD
		return "AMD";
	case 32902: // Intel
		return "Intel";
	case 5140: // Microsoft
		return "Microsoft";
	default:
		return "Desconocido";
	}
}