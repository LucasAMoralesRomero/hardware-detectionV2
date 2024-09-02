//incluimos el encabezado de la clase
#include "../cpu.h"
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

bool cpu::getCPUInfo(std::string& name, int& cores, int& logicalProcessors, double& maxClockSpeed)
{
	HRESULT hres;

	// Inicializar COM
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres)) {
		std::cerr << "Failed to initialize COM library. Error code = 0x"
			<< std::hex << hres << std::endl;
		return false;
	}

	// Configurar la seguridad
	hres = CoInitializeSecurity(
		NULL,
		-1,                          // Número de elementos de la lista de autenticación
		NULL,                        // Puntero a la lista de servicios de autenticación
		NULL,                        // Reservado
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Nivel de autenticación
		RPC_C_IMP_LEVEL_IMPERSONATE, // Nivel de autorización
		NULL,                        // Puntero a la lista de identidad de autenticación
		EOAC_NONE,                   // Capacidad adicional
		NULL                         // Reservado
	);

	if (FAILED(hres)) {
		std::cerr << "Failed to initialize security. Error code = 0x"
			<< std::hex << hres << std::endl;
		CoUninitialize();
		return false;
	}

	// Obtener el objeto IWbemLocator
	IWbemLocator *pLoc = NULL;
	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres)) {
		std::cerr << "Failed to create IWbemLocator object. "
			<< "Error code = 0x" << std::hex << hres << std::endl;
		CoUninitialize();
		return false;
	}

	// Conectar con el espacio de nombres WMI
	IWbemServices *pSvc = NULL;
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Espacio de nombres
		NULL,                    // Usuario
		NULL,                    // Contraseña
		0,                       // Locale
		NULL,                    // Flags de seguridad
		0,                       // Autoridad
		0,                       // Contexto
		&pSvc                    // IWbemServices
	);

	if (FAILED(hres)) {
		std::cerr << "Could not connect to WMI. Error code = 0x"
			<< std::hex << hres << std::endl;
		pLoc->Release();
		CoUninitialize();
		return false;
	}

	// Configurar la seguridad en el proxy
	hres = CoSetProxyBlanket(
		pSvc,                        // Proxy
		RPC_C_AUTHN_WINNT,           // Autenticación
		RPC_C_AUTHZ_NONE,            // Autorización
		NULL,                        // Nombre del servidor
		RPC_C_AUTHN_LEVEL_CALL,      // Nivel de autenticación
		RPC_C_IMP_LEVEL_IMPERSONATE, // Nivel de autorización
		NULL,                        // Identidad del cliente
		EOAC_NONE                    // Capacidades del proxy
	);

	if (FAILED(hres)) {
		std::cerr << "Could not set proxy blanket. Error code = 0x"
			<< std::hex << hres << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return false;
	}

	// Ejecutar la consulta WMI
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT Name, NumberOfCores, NumberOfLogicalProcessors, MaxClockSpeed FROM Win32_Processor"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres)) {
		std::cerr << "WMI Query failed. Error code = 0x"
			<< std::hex << hres << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return false;
	}

	// Obtener los resultados
	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator) {
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
		if (0 == uReturn) {
			break;
		}

		VARIANT vtProp;

		// Obtener el nombre del CPU
		hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR) {
			_bstr_t bstrName(vtProp.bstrVal);
			name = std::string((char*)bstrName);
		}
		VariantClear(&vtProp);

		// Obtener el número de núcleos físicos
		hr = pclsObj->Get(L"NumberOfCores", 0, &vtProp, 0, 0);
		if (SUCCEEDED(hr) && (vtProp.vt == VT_I4 || vtProp.vt == VT_UI4)) {
			cores = vtProp.intVal;
		}
		VariantClear(&vtProp);

		// Obtener el número de procesadores lógicos
		hr = pclsObj->Get(L"NumberOfLogicalProcessors", 0, &vtProp, 0, 0);
		if (SUCCEEDED(hr) && (vtProp.vt == VT_I4 || vtProp.vt == VT_UI4)) {
			logicalProcessors = vtProp.intVal;
		}
		VariantClear(&vtProp);

		// Obtener la velocidad máxima del reloj en MHz y convertir a GHz
		hr = pclsObj->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
		if (SUCCEEDED(hr) && (vtProp.vt == VT_I4 || vtProp.vt == VT_UI4)) {
			maxClockSpeed = static_cast<double>(vtProp.intVal) / 1000.0; // Convertir a GHz
		}
		VariantClear(&vtProp);

		pclsObj->Release();
	}

	// Limpieza
	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return true;
}