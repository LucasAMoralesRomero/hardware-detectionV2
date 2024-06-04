#include "screen.h"
#include <string>
using namespace std;

string screen::getScreen()
{
	//obtenemos el ancho de la pantalla
	width = GetSystemMetrics(SM_CXSCREEN);
	//obtenemeos el alto de la pantalla
	height = GetSystemMetrics(SM_CYSCREEN);
	return to_string(width) + "x" + to_string(height);
}
