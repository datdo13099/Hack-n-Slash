#ifndef GAME
#define GAME

#include "globals.h"
#include "hero.h"
#include "wall.h"
#include "glob.h"
#include "keyboardInput.h"
#include "drawing_functions.h"
#include "soundManager.h"
#include "CameraController.h"
#include "boss.h"
#include "hpBar.h"

#include <string>

// Định nghĩa lớp Game
class Game {
public:
    float fadeTimer = 0.0f; // Bộ đếm thời gian cho hiệu ứng mờ
    int fadeState = 0;      // Trạng thái mờ: 0 - Không mờ, 1 - Mờ dần vào, 2 - Mờ dần ra
    SDL_Texture* levelNameTexture = nullptr; // Texture hiển thị tên màn chơi

    std::string levelName; // Tên màn chơi
    int numGlobs = 0;      // Số lượng quái Glob
    int numGrobs = 0;      // Số lượng quái Grob
    int bossCount = 0;     // Số lượng boss

    bool gameWon = false;         // Trạng thái chiến thắng
    SDL_Texture* victoryImage;    // Texture cho màn hình chiến thắng

    Mix_Music* song; // Nhạc nền

    // Các tập hợp animation
    AnimationSet* heroAnimSet;    // Animation của nhân vật chính
    AnimationSet* globAnimSet;    // Animation của quái Glob
    AnimationSet* grobAnimSet;    // Animation của quái Grob
    AnimationSet* wallAnimSet;    // Animation của tường
    AnimationSet* bossAnimSet; // Animation của boss
    AnimationSet* bulletAnimSet;  // Animation của đạn

    // Các texture
    SDL_Texture* backgroundImage; // Ảnh nền
    SDL_Texture* splashImage;     // Ảnh splash
    SDL_Texture* overlayImage;    // Ảnh overlay

    SDL_Texture* scoreTexture = NULL; // Texture hiển thị điểm số

    Hero* hero; // Nhân vật chính
    KeyboardInput heroInput; // Đầu vào từ bàn phím cho nhân vật chính

    list<Entity*> enemies; // Danh sách các kẻ thù
    list<Entity*> walls;   // Danh sách các tường
    std::list<HPBar*> bossHealthBars; // Danh sách thanh máu của boss

    HPBar playerHpBar; // Thanh máu của người chơi
    SDL_Texture* pauseTexture = nullptr; // Texture cho màn hình tạm dừng
    SDL_Texture* muteTexture = nullptr;  // Texture cho trạng thái tắt tiếng
    CameraController camController; // Điều khiển camera

    // Các trạng thái của trò chơi
    bool splashShowing; // Trạng thái hiển thị splash
    bool isPaused = false; // Trạng thái tạm dừng
    bool isMuted = false;  // Trạng thái tắt tiếng
    bool bossActive = false; // Trạng thái boss đang hoạt động
    bool buildBossNext = false; // Trạng thái chuẩn bị tạo boss

    float overlayTimer; // Bộ đếm thời gian overlay
    float enemyBuildTimer = 0; // Bộ đếm thời gian tạo kẻ thù

    int currentRound = 0; // Vòng chơi hiện tại
    int enemiesToBuild = 0; // Số lượng kẻ thù cần tạo
    int enemiesBuilt = 0;   // Số lượng kẻ thù đã tạo
    int enemyWavesTillBoss = 0; // Số đợt kẻ thù trước khi xuất hiện boss
    int bossesKilled = 0;      // Số lượng boss đã bị tiêu diệt

    HPBar* hpBar; // Thanh máu

    // Constructor và Destructor
    Game();
    ~Game();

    // Các hàm thành viên
    void update(); // Cập nhật trạng thái trò chơi
    void draw();   // Vẽ trò chơi lên màn hình
    void setupNextRound(); // Thiết lập vòng chơi tiếp theo
    void setEnemyCounts(int round); // Thiết lập số lượng kẻ thù cho vòng chơi
    void resetGame(); // Đặt lại trò chơi
    int getBossCountForRound(int round); // Lấy số lượng boss cho vòng chơi
};

#endif
