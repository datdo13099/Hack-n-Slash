#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "cleanup.h"
#include "res_path.h"
#include "drawing_functions.h"
#include "globals.h"

int main(int argc, char** agv)
{
	//setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "Loi khoi tao SDL" << endl;
		return 1;
	}

	//setup window
	SDL_Window* window = SDL_CreateWindow("Proj 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		Globals::ScreenWidth*Globals::ScreenScale, Globals::ScreenHeight*Globals::ScreenScale
		, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		SDL_Quit();
		cout << "Loi cua so" << endl;
		return 1;
	}

	//setup renderer
	Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Globals::renderer == nullptr)
	{
		cleanup(window);
		SDL_Quit();
		cout << "Render loi" << endl;
		return -1;
	}

	SDL_RenderSetLogicalSize(Globals::renderer, Globals::ScreenWidth, Globals::ScreenHeight);

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Quit();
		cout << "SDL Image khong duoc khoi tao" << endl;
		return 1;
	}

	if (TTF_Init() != 0)
	{
		SDL_Quit();
		cout << "SDL Ttf khong duoc khoi tao" << endl;
		return 1;
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SDL_Quit();
		cout << "Mixer khong duoc khoi tao" << endl;
		return 1;
	}

	//load mot texture de ve
	string resPath = getResourcePath();
	SDL_Texture* texture = loadTexture(resPath + "map.png", Globals::renderer);

	//chay tro choi trong 5ms
	while (SDL_GetTicks() < 5000)
	{
		//xoa man hinh
		SDL_RenderClear(Globals::renderer);
		//hien thi nhung gi ta muon len man hinh
		renderTexture(texture, Globals::renderer, 0, 0);
		//show anh chung ta muon render
		SDL_RenderPresent(Globals::renderer);
	}

	cleanup(Globals::renderer);
	cleanup(window);
	cleanup(texture);

	SDL_Quit();
	return 0;
}