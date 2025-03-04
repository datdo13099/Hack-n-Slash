#ifndef LIVINGENTITY  // Kiểm tra nếu LIVINGENTITY chưa được định nghĩa
#define LIVINGENTITY  // Định nghĩa LIVINGENTITY để tránh include file nhiều lần

#include "entity.h"  // Bao gồm file header của lớp Entity, lớp cha của LivingEntity

// Lớp trừu tượng (abstract class)
class LivingEntity : public Entity  // Định nghĩa lớp LivingEntity, kế thừa công khai từ Entity
{
public:
    int hp, hpMax;  // Máu hiện tại (hp) và máu tối đa (hpMax) của thực thể sống
    int damage = 0;  // Sát thương mà thực thể này gây ra, mặc định là 0
    SDL_Rect hitBox;  // Hộp va chạm tương tự như collisionBox, nhưng mô tả vùng gây sát thương

    float invincibleTimer = 0;  // Bộ đếm thời gian bất tử, nếu > 0 thì không thể bị tổn thương

    virtual void updateHitBox();  // Hàm ảo cập nhật hộp gây sát thương (hitBox)
    virtual void updateDamages() = 0;  // Hàm ảo thuần túy: cách thực thể bị tổn thương bởi các yếu tố khác (do lớp con định nghĩa)
    virtual void die() = 0;  // Hàm ảo thuần túy: cách thực thể chết (do lớp con quyết định)
    virtual void updateInvincibleTimer();  // Hàm ảo cập nhật bộ đếm thời gian bất tử

    void draw();  // Ghi đè (override) hàm draw của lớp Entity để vẽ thực thể sống
};

#endif  // Kết thúc khối điều kiện định nghĩa LIVINGENTITY