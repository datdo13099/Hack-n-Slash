#include "cleanup.h"

/**
 * giải phóng bộ nhớ của cửa sổ SDL
 * @param win con trỏ đến SDL_Window cần giải phóng
 */
void cleanup(SDL_Window* win) 
{
	if (!win) 
	{
		return;
	}
	SDL_DestroyWindow(win);
}

/**
 * giải phóng bộ nhớ của renderer SDL
 * @param ren con trỏ đến SDL_Renderer cần giải phóng
 */
void cleanup(SDL_Renderer* ren) 
{
	if (!ren) 
	{
		return;
	}
	SDL_DestroyRenderer(ren);
}

/**
 * giải phóng bộ nhớ của texture SDL
 * @param tex con trỏ đến SDL_Texture cần giải phóng
 */
void cleanup(SDL_Texture* tex) 
{
	if (!tex) 
	{
		return;
	}
	SDL_DestroyTexture(tex);
}

/**
 * giải phóng bộ nhớ của surface SDL
 * @param surf con trỏ đến SDL_Surface cần giải phóng
 */
void cleanup(SDL_Surface* surf) 
{
	if (!surf) 
	{
		return;
	}
	SDL_FreeSurface(surf);
}