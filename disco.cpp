#include "disco.h"
#include <iostream>
#include <iomanip>
#include <fileapi.h>
#include <winioctl.h>

using namespace std;

// Constructor
Disco::Disco() {}

// Destructor
Disco::~Disco() {}

// Convierte de bytes a gigabytes (GB)
double convertirBytesAGB(ULONGLONG sizeInBytes) {
	return static_cast<double>(sizeInBytes) / (1024 * 1024 * 1024);  // Convertir de bytes a gigabytes
}

// Verifica si un disco es SSD usando DeviceIoControl
bool Disco::esSSD(const wstring& path) {
	HANDLE hDevice = CreateFileW(
		path.c_str(),
		0,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	if (hDevice == INVALID_HANDLE_VALUE) {
		return false;
	}

	STORAGE_PROPERTY_QUERY storagePropertyQuery;
	ZeroMemory(&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));
	storagePropertyQuery.PropertyId = StorageDeviceTrimProperty;
	storagePropertyQuery.QueryType = PropertyStandardQuery;

	DEVICE_TRIM_DESCRIPTOR trimDescriptor;
	ZeroMemory(&trimDescriptor, sizeof(DEVICE_TRIM_DESCRIPTOR));

	DWORD dwBytesReturned = 0;
	if (!DeviceIoControl(
		hDevice,
		IOCTL_STORAGE_QUERY_PROPERTY,
		&storagePropertyQuery,
		sizeof(STORAGE_PROPERTY_QUERY),
		&trimDescriptor,
		sizeof(DEVICE_TRIM_DESCRIPTOR),
		&dwBytesReturned,
		NULL)) {
		CloseHandle(hDevice);
		return false;
	}

	CloseHandle(hDevice);
	return trimDescriptor.TrimEnabled;  // Si está habilitado, es un SSD.
}

// Obtiene el tamaño del disco en bytes usando DeviceIoControl
ULONGLONG Disco::obtenerTamañoDisco(const wstring& path) {
	HANDLE hDevice = CreateFileW(
		path.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	if (hDevice == INVALID_HANDLE_VALUE) {
		wcerr << L"Error al abrir el dispositivo: " << path << endl;
		return 0;
	}

	DISK_GEOMETRY diskGeometry;
	DWORD bytesReturned;

	if (!DeviceIoControl(
		hDevice,
		IOCTL_DISK_GET_DRIVE_GEOMETRY,
		NULL,
		0,
		&diskGeometry,
		sizeof(diskGeometry),
		&bytesReturned,
		NULL)) {
		wcerr << L"Error al obtener la geometría del disco." << endl;
		CloseHandle(hDevice);
		return 0;
	}

	CloseHandle(hDevice);

	// Calcula el tamaño del disco en bytes
	ULONGLONG tamañoDisco = diskGeometry.Cylinders.QuadPart * diskGeometry.TracksPerCylinder *
		diskGeometry.SectorsPerTrack * diskGeometry.BytesPerSector;
	return tamañoDisco;
}

// Obtiene el nombre del modelo del disco
wstring obtenerModeloDisco(const wstring& path) {
	HANDLE hDevice = CreateFileW(
		path.c_str(),
		0,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	if (hDevice == INVALID_HANDLE_VALUE) {
		wcerr << L"Error al abrir el dispositivo para obtener el modelo: " << path << endl;
		return L"Desconocido";
	}

	STORAGE_PROPERTY_QUERY storagePropertyQuery;
	ZeroMemory(&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));
	storagePropertyQuery.PropertyId = StorageDeviceProperty;
	storagePropertyQuery.QueryType = PropertyStandardQuery;

	BYTE buffer[1024] = { 0 };
	DWORD dwBytesReturned = 0;

	if (!DeviceIoControl(
		hDevice,
		IOCTL_STORAGE_QUERY_PROPERTY,
		&storagePropertyQuery,
		sizeof(STORAGE_PROPERTY_QUERY),
		&buffer,
		sizeof(buffer),
		&dwBytesReturned,
		NULL)) {
		wcerr << L"Error al obtener el modelo del disco." << endl;
		CloseHandle(hDevice);
		return L"Desconocido";
	}

	CloseHandle(hDevice);

	STORAGE_DEVICE_DESCRIPTOR* deviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)buffer;

	// Extraer el nombre de la marca y modelo
	const char* vendorName = (const char*)(buffer + deviceDescriptor->VendorIdOffset);
	const char* productName = (const char*)(buffer + deviceDescriptor->ProductIdOffset);

	// Convertimos el nombre del modelo a wstring
	wstring marca = wstring(vendorName, vendorName + strlen(vendorName));
	wstring modelo = wstring(productName, productName + strlen(productName));

	// Devolver marca + modelo
	return marca + L" " + modelo;
}

// Obtiene la información de todos los discos duros conectados
void Disco::obtenerInfoDiscos() {
	// Recorrer hasta 10 discos físicos
	for (int i = 0; i < 10; ++i) {
		wstring drivePath = L"\\\\.\\PhysicalDrive" + to_wstring(i);

		// Intentamos abrir el disco
		HANDLE hDevice = CreateFileW(
			drivePath.c_str(),
			0,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
		);

		// Si no se puede abrir el dispositivo, continuamos con el siguiente
		if (hDevice == INVALID_HANDLE_VALUE) {
			continue;
		}

		CloseHandle(hDevice);

		// Obtener el modelo del disco (marca + modelo)
		wstring modeloDisco = obtenerModeloDisco(drivePath);

		// Verificar si es SSD
		bool isSSD = esSSD(drivePath);

		// Obtener el tamaño del disco en bytes
		ULONGLONG tamañoDiscoBytes = obtenerTamañoDisco(drivePath);

		// Mostrar la información del disco
		wcout << L"Modelo: " << modeloDisco << endl;
		wcout << L"Tipo de Disco: " << (isSSD ? L"SSD" : L"HDD") << endl;
		wcout << L"Tamaño: " << fixed << setprecision(2) << convertirBytesAGB(tamañoDiscoBytes) << L" GB" << endl;
		wcout << L"--------------------------------------" << endl;
	}
}
