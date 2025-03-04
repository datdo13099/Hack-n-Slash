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
    // khởi tạo SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        cout << "lỗi khởi tạo SDL" << endl;
        return 1;
    }
    // tạo cửa sổ
    SDL_Window* window = SDL_CreateWindow("Hack 'n' Slash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        Globals::ScreenWidth * Globals::ScreenScale, Globals::ScreenHeight * Globals::ScreenScale,
        SDL_WINDOW_SHOWN); // SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
    if (window == nullptr)
    {
        SDL_Quit();
        cout << "lỗi tạo cửa sổ" << endl;
        return 1;
    }

    // tạo bộ vẽ
    Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (Globals::renderer == nullptr)
    {
        cleanup(window);
        SDL_Quit();
        cout << "lỗi tạo bộ vẽ" << endl;
        return 1;
    }
    // thiết lập kích thước vẽ trước khi phóng to lên kích thước màn hình
    SDL_RenderSetLogicalSize(Globals::renderer, Globals::ScreenWidth, Globals::ScreenHeight);

    // khởi tạo SDL_image
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_Quit();
        cout << "lỗi khởi tạo SDL_image" << endl;
        return 1;
    }
    // khởi tạo SDL_ttf
    if (TTF_Init() != 0)
    {
        SDL_Quit();
        cout << "lỗi khởi tạo SDL_ttf" << endl;
        return 1;
    }
    // khởi tạo SDL_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        SDL_Quit();
        cout << "lỗi khởi tạo SDL_mixer" << endl;
        return 1;
    }

    // khởi tạo game và chạy vòng lặp cập nhật
    Game game;
    game.update();

    cleanup(Globals::renderer);
    cleanup(window);
    // cleanup(texture);

    SDL_Quit();
    return 0;
}
