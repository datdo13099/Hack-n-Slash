#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "cleanup.h"
#include "res_path.h" 
#include "drawing_functions.h"
#include "globals.h"
#include "game.h"

int main(int argc, char** agv)
{
    // khoi tao SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        cout << "loi khoi tao SDL" << endl;
        return 1;
    }

    // tao cua so
    SDL_Window* window = SDL_CreateWindow("Hack 'n' Slash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        Globals::ScreenWidth * Globals::ScreenScale, Globals::ScreenHeight * Globals::ScreenScale,
        SDL_WINDOW_SHOWN); // SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
    if (window == nullptr)
    {
        SDL_Quit();
        cout << "loi tao cua so" << endl;
        return 1;
    }

    // tao bo ve
    Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (Globals::renderer == nullptr)
    {
        cleanup(window);
        SDL_Quit();
        cout << "loi tao bo ve" << endl;
        return 1;
    }

    // thiet lap kich thuoc ve truoc khi phong to len kich thuoc man hinh
    SDL_RenderSetLogicalSize(Globals::renderer, Globals::ScreenWidth, Globals::ScreenHeight);

    // khoi tao SDL_image
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_Quit();
        cout << "loi khoi tao SDL_image" << endl;
        return 1;
    }

    // khoi tao SDL_ttf
    if (TTF_Init() != 0)
    {
        SDL_Quit();
        cout << "loi khoi tao SDL_ttf" << endl;
        return 1;
    }

    // khoi tao SDL_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        SDL_Quit();
        cout << "loi khoi tao SDL_mixer" << endl;
        return 1;
    }

    // khoi tao game va chay vong lap cap nhat
    Game game;
    game.update();

    cleanup(Globals::renderer);
    cleanup(window);
    // cleanup(texture);

    SDL_Quit();
    return 0;
}