#ifndef GAME
#define GAME

#include "globals.h"            // Include file header chứa các biến toàn cục
#include "hero.h"               // Include file header của lớp Hero
#include "wall.h"               // Include file header của lớp Wall
#include "glob.h"               // Include file header của lớp Glob
#include "keyboardInput.h"      // Include file header xử lý đầu vào từ bàn phím
#include "drawing_functions.h"  // Include file header chứa các hàm vẽ
#include "soundManager.h"

// Định nghĩa lớp Game
class Game
{
public:
    Mix_Music* song;

    // Khai báo các con trỏ tới tập hợp animation
    AnimationSet* heroAnimSet;  // Tập hợp animation cho nhân vật chính (hero)
    AnimationSet* globAnimSet;  // Tập hợp animation cho kẻ địch (glob)
    AnimationSet* wallAnimSet;  // Tập hợp animation cho tường (wall)

    SDL_Texture* backgroundImage; // con trỏ tới texture của hình nền chính
    SDL_Texture* splashImage; // con trỏ tới texture của hình splash 
    SDL_Texture* overlayImage; // con trỏ tới texture của lớp phủ

    SDL_Texture* scoreTexture = NULL; // con trỏ tới texture của điểm số

    Hero* hero;                   // Con trỏ tới đối tượng nhân vật chính
    KeyboardInput heroInput;      // Đối tượng xử lý đầu vào từ bàn phím cho hero

    list<Entity*> enemies;        // Danh sách các thực thể kẻ địch
    list<Entity*> walls;          // Danh sách các thực thể tường

    bool splashShowing;
    float overlayTimer;

    // Khai báo các hàm thành viên
    Game();                       // Hàm khởi tạo trò chơi
    ~Game();                      // Hàm hủy trò chơi

    void update();                // Hàm cập nhật trạng thái trò chơi
    void draw();                  // Hàm vẽ trò chơi lên màn hình
};

#endif