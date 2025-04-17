#ifndef BOSS
#define BOSS

#include <cstdlib>
#include "livingEntity.h"
#include "soundManager.h"
#include "bullet.h"

// Định nghĩa lớp Boss, kế thừa từ LivingEntity
class Boss : public LivingEntity
{
public:
    // Các animation
    static const string BOSS_ANIM_IDLE;           // Animation trạng thái đứng yên
    static const string BOSS_ANIM_CHARGE;         // Animation trạng thái tích năng lượng
    static const string BOSS_ANIM_SHOOT;          // Animation trạng thái bắn
    static const string BOSS_ANIM_SLAM;           // Animation trạng thái đập
    static const string BOSS_ANIM_JUMP_TELEGRAPH; // Animation trạng thái chuẩn bị nhảy
    static const string BOSS_ANIM_JUMP;           // Animation trạng thái nhảy
    static const string BOSS_ANIM_DIE;            // Animation trạng thái chết

    // Các trạng thái (state)
    static const int BOSS_STATE_IDLE;             // Trạng thái đứng yên
    static const int BOSS_STATE_CHARGE;           // Trạng thái tích năng lượng
    static const int BOSS_STATE_SHOOT;            // Trạng thái bắn
    static const int BOSS_STATE_SLAM;             // Trạng thái đập
    static const int BOSS_STATE_JUMP_TELEGRAPH;   // Trạng thái chuẩn bị nhảy
    static const int BOSS_STATE_JUMP;             // Trạng thái nhảy
    static const int BOSS_STATE_DEAD;             // Trạng thái chết

    // Các giai đoạn (phase)
    static const int BOSS_PHASE_NORMAL;           // Giai đoạn bình thường
    static const int BOSS_PHASE_DAMAGED;          // Giai đoạn bị thương
    static const int BOSS_PHASE_FRANTIC;          // Giai đoạn hoảng loạn

    // Khi bằng 0, thực hiện hành động mới
    float thinkTimer = 0; // Bộ đếm thời gian suy nghĩ
    float shootTimer = 0; // Thời gian bắn
    float shotTimer = 0;  // Thời gian giữa các viên đạn

    LivingEntity* target = NULL; // Mục tiêu mà boss sẽ đuổi theo

    int aiState = BOSS_PHASE_NORMAL; // Giai đoạn hiện tại của boss

    AnimationSet* bulletAnimSet; // Tham chiếu đến animation của đạn

    static int bossesKilled; // Số lượng Boss đã bị tiêu diệt

    // Constructor
    Boss(AnimationSet* animSet, AnimationSet* bulletAnimSet);

    // Các hàm thành viên
    void update();                // Cập nhật trạng thái của Boss
    void updateShoot();           // Cập nhật trạng thái bắn
    void think();                 // Suy nghĩ để quyết định hành động tiếp theo
    void charge();                // Hành động tích năng lượng
    void shoot();                 // Hành động bắn
    void slam();                  // Hành động đập
    void jumpTelegraph();         // Hành động chuẩn bị nhảy
    void jump();                  // Hành động nhảy
    void die();                   // Hành động chết
    void findNearestTarget();     // Tìm mục tiêu gần nhất
    void changeAnimation(int newState, bool resetFrameToBeginning); // Thay đổi animation
    void updateAnimation();       // Cập nhật animation
    void updateDamages();         // Cập nhật sát thương
};

#endif