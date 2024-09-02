#pragma once
#include <string>

class cpu {
public:
	// Función que obtiene la información del CPU
	bool getCPUInfo(std::string& name, int& cores, int& logicalProcessors, double& maxClockSpeed);
};