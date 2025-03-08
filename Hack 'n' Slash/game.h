#ifndef GAME
#define GAME

#include "globals.h"            // Include file header chứa các biến toàn cục
#include "hero.h"               // Include file header của lớp Hero
#include "wall.h"               // Include file header của lớp Wall
#include "glob.h"               // Include file header của lớp Glob
#include "keyboardInput.h"      // Include file header xử lý đầu vào từ bàn phím
#include "drawing_functions.h"  // Include file header chứa các hàm vẽ

// Định nghĩa lớp Game
class Game
{
public:
    // Khai báo các con trỏ tới tập hợp animation
    AnimationSet* heroAnimSet;  // Tập hợp animation cho nhân vật chính (hero)
    AnimationSet* globAnimSet;  // Tập hợp animation cho kẻ địch (glob)
    AnimationSet* wallAnimSet;  // Tập hợp animation cho tường (wall)

    SDL_Texture* backgroundImage; // Con trỏ tới texture của hình nền

    Hero* hero;                   // Con trỏ tới đối tượng nhân vật chính
    KeyboardInput heroInput;      // Đối tượng xử lý đầu vào từ bàn phím cho hero

    list<Entity*> enemies;        // Danh sách các thực thể kẻ địch
    list<Entity*> walls;          // Danh sách các thực thể tường

    // Khai báo các hàm thành viên
    Game();                       // Hàm khởi tạo trò chơi
    ~Game();                      // Hàm hủy trò chơi

    void update();                // Hàm cập nhật trạng thái trò chơi
    void draw();                  // Hàm vẽ trò chơi lên màn hình
};

#endif