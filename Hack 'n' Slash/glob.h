#ifndef GLOB
#define GLOB

#include <cstdlib>              // Thư viện chuẩn C để sử dụng rand()
#include "livingEntity.h"       // Include file header của lớp LivingEntity
#include "soundManager.h"       // Thêm âm thanh

// Định nghĩa lớp Glob, kế thừa từ LivingEntity
class Glob : public LivingEntity
{
public:
    // Khai báo các hằng số tĩnh cho animation của Glob
    static const string GLOB_ANIM_UP;              // Animation đi lên
    static const string GLOB_ANIM_DOWN;            // Animation đi xuống
    static const string GLOB_ANIM_LEFT;            // Animation sang trái
    static const string GLOB_ANIM_RIGHT;           // Animation sang phải
    static const string GLOB_ANIM_IDLE_UP;         // Animation đứng yên hướng lên
    static const string GLOB_ANIM_IDLE_DOWN;       // Animation đứng yên hướng xuống
    static const string GLOB_ANIM_IDLE_LEFT;       // Animation đứng yên hướng trái
    static const string GLOB_ANIM_IDLE_RIGHT;      // Animation đứng yên hướng phải
    static const string GLOB_ANIM_ATTACK_UP;       // Animation tấn công hướng lên
    static const string GLOB_ANIM_ATTACK_DOWN;     // Animation tấn công hướng xuống
    static const string GLOB_ANIM_ATTACK_LEFT;     // Animation tấn công hướng trái
    static const string GLOB_ANIM_ATTACK_RIGHT;    // Animation tấn công hướng phải
    static const string GLOB_ANIM_TELEGRAPH_UP;    // Animation báo trước hướng lên
    static const string GLOB_ANIM_TELEGRAPH_DOWN;  // Animation báo trước hướng xuống
    static const string GLOB_ANIM_TELEGRAPH_LEFT;  // Animation báo trước hướng trái
    static const string GLOB_ANIM_TELEGRAPH_RIGHT; // Animation báo trước hướng phải
    static const string GLOB_ANIM_DIE;             // Animation khi chết

    // Khai báo các hằng số tĩnh cho trạng thái của Glob
    static const int GLOB_STATE_IDLE;              // Trạng thái đứng yên
    static const int GLOB_STATE_MOVE;              // Trạng thái di chuyển
    static const int GLOB_STATE_ATTACK;            // Trạng thái tấn công
    static const int GLOB_STATE_TELEGRAPH;         // Trạng thái báo trước tấn công
    static const int GLOB_STATE_DEAD;              // Trạng thái đã chết

    // Khai báo các hằng số tĩnh cho AI của Glob
    static const int GLOB_AI_NORMAL;               // Trạng thái AI bình thường
    static const int GLOB_AI_CHASE;                // Trạng thái AI đuổi theo mục tiêu

    static int globsKilled;                        // Biến tĩnh đếm số Glob đã bị tiêu diệt

    // Biến thành viên
    float thinkTimer = 0;                          // Thời gian suy nghĩ, khi bằng 0 thì chọn hành động AI
    LivingEntity* target = NULL;                   // Con trỏ tới mục tiêu cần đuổi theo
    int aiState = GLOB_AI_NORMAL;                  // Trạng thái AI hiện tại, mặc định là bình thường

    // Khai báo các hàm thành viên
    Glob(AnimationSet* animSet);                   // Hàm khởi tạo với tập hợp animation
    void update();                                 // Cập nhật trạng thái của Glob mỗi frame
    void think();                                  // Xử lý logic AI
    void telegraph();                              // Báo trước khi tấn công
    void attack();                                 // Thực hiện tấn công
    void die();                                    // Xử lý khi Glob chết
    void findNearestTarget();                      // Tìm mục tiêu gần nhất
    void changeAnimation(int newState, bool resetFrameToBeginning); // Thay đổi animation
    void updateAnimation();                        // Cập nhật animation
    void updateDamages();                          // Cập nhật sát thương nhận vào
};

#endif