#ifndef GROB
#define GROB

#include <cstdlib> // Cần cho số ngẫu nhiên
#include "livingEntity.h"
#include "soundManager.h"

class Grob : public LivingEntity
{
public:
    static const string GROB_ANIM_UP;
    static const string GROB_ANIM_DOWN;
    static const string GROB_ANIM_LEFT;
    static const string GROB_ANIM_RIGHT;
    static const string GROB_ANIM_IDLE_UP;
    static const string GROB_ANIM_IDLE_DOWN;
    static const string GROB_ANIM_IDLE_LEFT;
    static const string GROB_ANIM_IDLE_RIGHT;
    static const string GROB_ANIM_DIE;

    static const int GROB_STATE_IDLE;
    static const int GROB_STATE_MOVE;
    static const int GROB_STATE_DEAD;

    static int grobsKilled;

    // Khi giá trị này là 0, nghĩ ra việc mới để làm (dùng cho AI)
    float thinkTimer = 0;

    // Các hàm chính
    Grob(AnimationSet* animSet);
    void update();
    void think();
    void die();
    void changeAnimation(int newState, bool resetFrameToBegging);

    void updateAnimation();
    void updateHitBox();
    void updateDamages();
};

#endif