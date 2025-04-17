#ifndef BULLET
#define BULLET

#include <cstdlib>
#include "livingEntity.h"
#include "soundManager.h"

// Lớp Bullet: Đại diện cho đạn trong trò chơi
class Bullet : public LivingEntity 
{
public:
    static const string BULLET_ANIM_BULLET; // Tên animation cho đạn

    // Constructor: Khởi tạo đạn với animation và tọa độ ban đầu
    Bullet(AnimationSet* animSet, int x, int y);

    void update(); // Hàm cập nhật trạng thái của đạn
    void die() {}; // Hàm xử lý khi đạn "chết" (không sử dụng)
    void changeAnimation(int newState, bool resetFrameToBeginning); // Thay đổi animation
    void updateAnimation(); // Cập nhật animation
    void updateDamages() {}; // Cập nhật sát thương (không sử dụng)
    void hitLanded(LivingEntity* entity); // Xử lý khi đạn trúng thực thể khác
    virtual void crashOntoSolid(); // Xử lý khi đạn va chạm với vật thể rắn
};

#endif
