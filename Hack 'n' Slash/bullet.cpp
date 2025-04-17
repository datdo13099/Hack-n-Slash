#include "bullet.h"

// Tên animation cho đạn
const string Bullet::BULLET_ANIM_BULLET = "bullet";

// Constructor: Khởi tạo đối tượng Bullet
Bullet::Bullet(AnimationSet* animSet, int x, int y) 
{
    this->animSet = animSet; // Gán tập hợp animation cho đạn

    solid = false; // Đạn không phải vật thể rắn
    collideWithSolids = true; // Đạn có thể va chạm với các vật thể rắn
    dieOnSolids = true; // Đạn sẽ biến mất khi va chạm với vật thể rắn
    moving = true; // Đạn luôn di chuyển
    type = "enemy"; // Loại của đạn là "enemy" (kẻ thù)

    this->x = x; // Tọa độ x ban đầu của đạn
    this->y = y; // Tọa độ y ban đầu của đạn
    moveSpeed = 40; // Tốc độ di chuyển của đạn
    moveSpeedMax = 40; // Tốc độ tối đa của đạn
    hp = hpMax = 1; // Máu của đạn (1 máu)
    damage = 5; // Sát thương của đạn
    collisionBoxW = 5; // Chiều rộng hộp va chạm
    collisionBoxH = 5; // Chiều cao hộp va chạm
    collisionBox.w = collisionBoxW; // Gán chiều rộng hộp va chạm
    collisionBox.h = collisionBoxH; // Gán chiều cao hộp va chạm
    collisionBoxYOffset = 0; // Độ lệch hộp va chạm theo trục y

    direction = DIR_DOWN; // Hướng di chuyển mặc định là xuống

    changeAnimation(0, true); // Đặt animation ban đầu

    updateCollisionBox(); // Cập nhật hộp va chạm

    invincibleTimer = 0; // Bộ đếm thời gian bất tử (không sử dụng)
}

// Hàm cập nhật trạng thái của đạn
void Bullet::update() 
{
    updateCollisionBox(); // Cập nhật hộp va chạm

    move(angle); // Di chuyển đạn theo góc
    updateMovement(); // Cập nhật chuyển động

    updateCollisions(); // Xử lý va chạm

    updateHitBox(); // Cập nhật hộp sát thương
    updateDamages(); // Cập nhật sát thương

    updateAnimation(); // Cập nhật animation
}

// Thay đổi animation của đạn
void Bullet::changeAnimation(int newState, bool resetFrameToBeginning) 
{
    state = newState; // Cập nhật trạng thái
    currentAnim = animSet->getAnimation(BULLET_ANIM_BULLET); // Lấy animation "bullet"

    if (resetFrameToBeginning)
        currentFrame = currentAnim->getFrame(0); // Đặt frame đầu tiên
    else
        currentFrame = currentAnim->getFrame(currentFrame->frameNumber); // Giữ nguyên frame hiện tại
}

// Cập nhật animation của đạn
void Bullet::updateAnimation() 
{
    if (currentFrame == NULL || currentAnim == NULL)
        return; // Nếu không có frame hoặc animation, thoát

    frameTimer += TimeController::timeController.dT; // Tăng thời gian frame

    if (frameTimer >= currentFrame->duration) { // Nếu thời gian frame vượt quá thời lượng
        if (currentFrame->frameNumber == currentAnim->getEndFrameNumber())
            currentFrame = currentAnim->getFrame(0); // Quay lại frame đầu tiên
        else
            currentFrame = currentAnim->getNextFrame(currentFrame); // Chuyển sang frame tiếp theo

        frameTimer = 0; // Đặt lại bộ đếm thời gian frame
    }
}

// Xử lý khi đạn trúng một thực thể khác
void Bullet::hitLanded(LivingEntity* entity)
{
    active = false; // Đạn ngừng hoạt động
}

// Xử lý khi đạn va chạm với vật thể rắn
void Bullet::crashOntoSolid() 
{
    active = false; // Đạn ngừng hoạt động
}
