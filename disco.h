#pragma once
#include <string>
#include <windows.h>
#include <vector>

using namespace std;

class Disco {
private:
	wstring modelo;
	wstring tipo;
	wstring tamaño;

public:
	Disco();
	~Disco();

	// Métodos para obtener la información de los discos
	void obtenerInfoDiscos();

	// Getters
	wstring getModelo() const;
	wstring getTipo() const;
	wstring getTamaño() const;

private:
	bool esSSD(const wstring& path);
	ULONGLONG obtenerTamañoDisco(const wstring& path);
};