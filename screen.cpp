#include "screen.h"

string screen::getScreen()
{
	//obtenemos el ancho de la pantalla
	width = GetSystemMetrics(SM_CXSCREEN);
	//obtenemeos el alto de la pantalla
	height = GetSystemMetrics(SM_CYSCREEN);
	return (width + "x" + height);
}
