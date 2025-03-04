#include "keyboardInput.h"

KeyboardInput::KeyboardInput()
{
    // gán cứng các nút bàn phím
    UP = SDL_SCANCODE_W;    // phím W cho hướng lên
    DOWN = SDL_SCANCODE_S;  // phím S cho hướng xuống
    LEFT = SDL_SCANCODE_A;  // phím A cho hướng trái
    RIGHT = SDL_SCANCODE_D; // phím D cho hướng phải
    SLASH = SDL_SCANCODE_N;
    DASH = SDL_SCANCODE_M;
    // nhiệm vụ bổ sung: tải mã quét từ một tệp văn bản
    // int test = 82;
    // UP = (SDL_Scancode)test;
}
void KeyboardInput::update(SDL_Event* e)
{
    // xử lý phím bấm xuống
    if (e->type == SDL_KEYDOWN)
    {
        if (e->key.keysym.scancode == DASH)
        {
            hero->dash();
        }
        if (e->key.keysym.scancode == SLASH)
        {
            hero->slash();
        }
    }

    // xử lý phím giữ
    // kiểm tra các phím vẫn đang được nhấn
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    // nếu nhân vật không thể di chuyển hoặc không có phím điều hướng nào đang được giữ, dừng di chuyển
    if ((hero->state != Hero::HERO_STATE_MOVE && hero->state != Hero::HERO_STATE_IDLE) || (!keystates[UP] && !keystates[DOWN] && !keystates[LEFT] && !keystates[RIGHT]))
    {
        hero->moving = false;
    }
    else
    {
        // di chuyển lên
        if (keystates[UP])
        {
            // lên phải
            if (keystates[RIGHT])
                hero->move(270 + 45);
            else if (keystates[LEFT])
                hero->move(270 - 45);
            else
                hero->move(270);
        }
        // di chuyển xuống
        if (keystates[DOWN])
        {
            // xuống phải
            if (keystates[RIGHT])
                hero->move(90 - 45);
            else if (keystates[LEFT])
                hero->move(90 + 45);
            else
                hero->move(90);
        }
        // di chuyển trái
        if (!keystates[UP] && !keystates[DOWN] && !keystates[RIGHT] && keystates[LEFT])
            hero->move(180);
        // di chuyển phải
        if (!keystates[UP] && !keystates[DOWN] && keystates[RIGHT] && !keystates[LEFT])
            hero->move(0);
    }
}