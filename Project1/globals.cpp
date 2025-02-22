#include "globals.h"

const float Globals::PI = 3.14159;

bool Globals::debugging = true;

int Globals::ScreenWidth = 640, Globals::ScreenHeight = 352, Globals::ScreenScale = 1;
SDL_Renderer* Globals::renderer = NULL;

string Globals::clipOffDataHeader(string data)
{
	int pos = data.find(":", 0); //return neu tim thay : trong string, nguoc lai, return -1
	if (pos != -1)
	{
		data = data.substr(pos + 1, data.length() - pos + 2);
	}
	return data;
}