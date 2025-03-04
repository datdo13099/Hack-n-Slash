#ifndef GAME  // Kiểm tra nếu GAME chưa được định nghĩa
#define GAME  // Định nghĩa GAME để tránh include file nhiều lần

#include "globals.h"          // Bao gồm file header chứa các định nghĩa toàn cục
#include "hero.h"             // Bao gồm file header của lớp Hero
#include "wall.h"             // Bao gồm file header của lớp Wall
#include "keyboardInput.h"    // Bao gồm file header của lớp KeyboardInput
#include "drawing_functions.h" // Bao gồm file header chứa các hàm hỗ trợ vẽ

class Game  // Định nghĩa lớp Game để quản lý toàn bộ trò chơi
{
public:
    AnimationSet* heroAnimSet;  // Tập hợp animation cho Hero
    AnimationSet* wallAnimSet;  // Tập hợp animation cho Wall

    SDL_Texture* backgroundImage;  // Texture của hình nền trò chơi

    Hero* hero;            // Con trỏ tới đối tượng Hero (nhân vật chính)
    KeyboardInput heroInput;  // Đối tượng KeyboardInput để điều khiển Hero

    list<Entity*> walls;  // Danh sách các con trỏ tới Entity đại diện cho các bức tường

    Game();   // Hàm khởi tạo của lớp Game
    ~Game();  // Hàm hủy để giải phóng tài nguyên

    void update();  // Hàm cập nhật trạng thái trò chơi mỗi frame
    void draw();    // Hàm vẽ các thành phần của trò chơi lên màn hình
};

#endif  // Kết thúc khối điều kiện định nghĩa GAME