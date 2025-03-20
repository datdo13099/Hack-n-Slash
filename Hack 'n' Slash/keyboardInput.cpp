#include "keyboardInput.h"
#include <iostream> // Include the iostream header
#include <vector>  // Include the vector header

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
        // Thêm phím bấm vào bộ đệm
        keyBuffer.push_back(e->key.keysym.scancode);
        // Giới hạn kích thước bộ đệm
        if (keyBuffer.size() > 4)
        {
            keyBuffer.pop_front();
        }
        // Kiểm tra mã Easter egg
        checkEasterEgg();

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
        hero->moving = true;
        // di chuyển lên
        if (keystates[UP])
        {
            if (keystates[RIGHT])
                hero->move(315); // lên phải
            else if (keystates[LEFT])
                hero->move(225); // lên trái
            else
                hero->move(270); // lên
        }
        // di chuyển xuống
        else if (keystates[DOWN])
        {
            if (keystates[RIGHT])
                hero->move(45); // xuống phải
            else if (keystates[LEFT])
                hero->move(135); // xuống trái
            else
                hero->move(90); // xuống
        }
        // di chuyển trái
        else if (keystates[LEFT])
        {
            hero->move(180); // trái
        }
        // di chuyển phải
        else if (keystates[RIGHT])
        {
            hero->move(0); // phải
        }
    }
}

void KeyboardInput::checkEasterEgg()
{
    // Mã Easter egg là "0000"
    const std::vector<SDL_Scancode> easterEggCode =
    {
        SDL_SCANCODE_0, SDL_SCANCODE_0, SDL_SCANCODE_0, SDL_SCANCODE_0
    };

    // Kiểm tra nếu bộ đệm khớp với mã Easter egg
    if (std::vector<SDL_Scancode>(keyBuffer.begin(), keyBuffer.end()) == easterEggCode)
    {
        hero->invincibleTimer = 99999999999999; // Đặt thời gian bất tử rất lớn
        hero->damage = 99999999999999; // Tăng sát thương tối đa
        std::cout << "Easter egg activated: hero damage = " << hero->damage << std::endl; // Add this line
    }
}