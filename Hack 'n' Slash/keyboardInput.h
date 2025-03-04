#ifndef KEYBOARDINPUT  // Kiểm tra nếu KEYBOARDINPUT chưa được định nghĩa
#define KEYBOARDINPUT  // Định nghĩa KEYBOARDINPUT để tránh include file nhiều lần

#include "hero.h"  // Bao gồm file header của lớp Hero, để liên kết với đối tượng Hero

class KeyboardInput  // Định nghĩa lớp KeyboardInput để xử lý đầu vào từ bàn phím
{
public:
    Hero* hero;  // Con trỏ tới đối tượng Hero mà lớp này điều khiển
    SDL_Scancode UP, DOWN, LEFT, RIGHT;  // Các mã phím (scancode) cho các hướng di chuyển
    SDL_Scancode SLASH, DASH;  // Các mã phím cho hành động tấn công (slash) và lướt nhanh (dash)

    KeyboardInput();  // Hàm khởi tạo của KeyboardInput
    void update(SDL_Event* e);  // Hàm cập nhật trạng thái đầu vào từ bàn phím, nhận sự kiện SDL làm tham số
};

#endif  // Kết thúc khối điều kiện định nghĩa KEYBOARDINPUT