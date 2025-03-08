#include "glob.h"

// Định nghĩa các hằng số chuỗi cho animation của Glob
const string Glob::GLOB_ANIM_UP = "up";                   // Animation đi lên
const string Glob::GLOB_ANIM_DOWN = "down";               // Animation đi xuống
const string Glob::GLOB_ANIM_LEFT = "left";               // Animation sang trái
const string Glob::GLOB_ANIM_RIGHT = "right";             // Animation sang phải
const string Glob::GLOB_ANIM_IDLE_UP = "idleUp";          // Animation đứng yên hướng lên
const string Glob::GLOB_ANIM_IDLE_DOWN = "idleDown";      // Animation đứng yên hướng xuống
const string Glob::GLOB_ANIM_IDLE_LEFT = "idleLeft";      // Animation đứng yên hướng trái
const string Glob::GLOB_ANIM_IDLE_RIGHT = "idleRight";    // Animation đứng yên hướng phải
const string Glob::GLOB_ANIM_ATTACK_UP = "attackUp";      // Animation tấn công hướng lên
const string Glob::GLOB_ANIM_ATTACK_DOWN = "attackDown";  // Animation tấn công hướng xuống
const string Glob::GLOB_ANIM_ATTACK_LEFT = "attackLeft";  // Animation tấn công hướng trái
const string Glob::GLOB_ANIM_ATTACK_RIGHT = "attackRight"; // Animation tấn công hướng phải
const string Glob::GLOB_ANIM_TELEGRAPH_UP = "telegraphUp"; // Animation báo trước hướng lên
const string Glob::GLOB_ANIM_TELEGRAPH_DOWN = "telegraphDown"; // Animation báo trước hướng xuống
const string Glob::GLOB_ANIM_TELEGRAPH_LEFT = "telegraphLeft"; // Animation báo trước hướng trái
const string Glob::GLOB_ANIM_TELEGRAPH_RIGHT = "telegraphRight"; // Animation báo trước hướng phải
const string Glob::GLOB_ANIM_DIE = "die";                 // Animation khi chết

// Định nghĩa các hằng số trạng thái của Glob
const int Glob::GLOB_STATE_IDLE = 0;                      // Trạng thái đứng yên
const int Glob::GLOB_STATE_MOVE = 1;                      // Trạng thái di chuyển
const int Glob::GLOB_STATE_ATTACK = 2;                    // Trạng thái tấn công
const int Glob::GLOB_STATE_TELEGRAPH = 3;                 // Trạng thái báo trước tấn công
const int Glob::GLOB_STATE_DEAD = 4;                      // Trạng thái đã chết

// Định nghĩa các hằng số AI của Glob
const int Glob::GLOB_AI_NORMAL = 0;                       // Trạng thái AI bình thường
const int Glob::GLOB_AI_CHASE = 1;                        // Trạng thái AI đuổi theo mục tiêu

int Glob::globsKilled = 0;                                // Biến tĩnh đếm số Glob đã bị tiêu diệt

// Hàm khởi tạo Glob
Glob::Glob(AnimationSet* animSet)
{
    this->animSet = animSet;                              // Gán tập hợp animation cho Glob

    type = "enemy";                                       // Xác định loại thực thể là kẻ địch

    // Thiết lập các giá trị mặc định
    x = Globals::ScreenWidth / 2;                         // Vị trí x ở giữa màn hình
    y = Globals::ScreenHeight / 2;                        // Vị trí y ở giữa màn hình
    moveSpeed = 0;                                        // Tốc độ di chuyển ban đầu
    moveSpeedMax = 20;                                    // Tốc độ tối đa
    hp = hpMax = 10 + (rand() % 20);                     // Máu ngẫu nhiên từ 10-29
    damage = 0;                                           // Sát thương mặc định
    collisionBoxW = 18;                                   // Chiều rộng hộp va chạm
    collisionBoxH = 20;                                   // Chiều cao hộp va chạm
    collisionBox.w = collisionBoxW;                       // Gán chiều rộng cho hộp va chạm
    collisionBox.h = collisionBoxH;                       // Gán chiều cao cho hộp va chạm

    collisionBoxYOffset = -14;                            // Độ lệch hộp va chạm theo trục y

    direction = DIR_DOWN;                                 // Hướng mặc định là xuống
    changeAnimation(GLOB_STATE_IDLE, true);              // Chuyển sang animation đứng yên

    updateCollisionBox();                                 // Cập nhật hộp va chạm
}

// Hàm cập nhật trạng thái của Glob mỗi frame
void Glob::update()
{
    // Kiểm tra nếu đã chết
    if (hp < 1 && state != GLOB_STATE_DEAD)
    {
        changeAnimation(GLOB_STATE_DEAD, true);          // Chuyển sang animation chết
        moving = false;                                   // Dừng di chuyển
        die();                                            // Gọi hàm xử lý khi chết
    }

    think();                                              // Xử lý logic AI

    updateCollisionBox();                                 // Cập nhật hộp va chạm
    updateMovement();                                     // Cập nhật di chuyển
    updateCollisions();                                   // Cập nhật va chạm
    updateHitBox();                                       // Cập nhật hộp đánh
    updateDamages();                                      // Cập nhật sát thương nhận vào

    updateAnimation();                                    // Cập nhật animation
    updateInvincibleTimer();                              // Cập nhật thời gian bất tử
}

// Hàm xử lý logic AI của Glob
void Glob::think()
{
    if (state == GLOB_STATE_IDLE || state == GLOB_STATE_MOVE)
    {
        thinkTimer -= TimeController::timeController.dT;  // Giảm thời gian suy nghĩ
        // Khi hết thời gian suy nghĩ, chọn hành động mới
        if (thinkTimer <= 0)
        {
            thinkTimer = rand() % 5;                      // Đặt lại thời gian ngẫu nhiên (0-5 giây)
            int action = rand() % 10;                     // Chọn hành động ngẫu nhiên (0-9)

            if (action < 3)
            {
                moving = false;                           // Dừng di chuyển
                aiState = GLOB_AI_NORMAL;                 // Chuyển về trạng thái AI bình thường
                changeAnimation(GLOB_STATE_IDLE, true);  // Chuyển sang animation đứng yên
            }
            else
            {
                findNearestTarget();                      // Tìm mục tiêu gần nhất
                // Nếu tìm thấy mục tiêu còn sống
                if (target != NULL && target->hp > 0)
                {
                    float dist = Entity::distanceBetweenTwoEntities(this, target); // Tính khoảng cách tới mục tiêu
                    // Nếu trong phạm vi tấn công
                    if (dist < 100)
                    {
                        telegraph();                      // Báo trước tấn công
                        aiState = GLOB_AI_NORMAL;         // Chuyển về trạng thái AI bình thường
                    }
                    else
                    {
                        // Nếu không trong phạm vi, đuổi theo mục tiêu
                        aiState = GLOB_AI_CHASE;          // Chuyển sang trạng thái AI đuổi theo
                        moving = true;                    // Bắt đầu di chuyển
                        changeAnimation(GLOB_STATE_MOVE, state != GLOB_STATE_MOVE); // Chuyển sang animation di chuyển
                    }
                }
                else
                {
                    // Không có mục tiêu, chuyển sang đứng yên
                    moving = false;                       // Dừng di chuyển
                    aiState = GLOB_AI_NORMAL;             // Chuyển về trạng thái AI bình thường
                    changeAnimation(GLOB_STATE_IDLE, true); // Chuyển sang animation đứng yên
                }
            }
        }
    }
    // Nếu đang đuổi theo mục tiêu
    if (aiState == GLOB_AI_CHASE && hp > 0 && active)
    {
        angle = Entity::angleBetweenTwoEntities(this, target); // Tính góc tới mục tiêu
        move(angle);                                          // Di chuyển theo góc đó
    }
}

// Hàm báo trước khi tấn công
void Glob::telegraph()
{
    moving = false;                                       // Dừng di chuyển
    frameTimer = 0;                                       // Đặt lại thời gian frame
    changeAnimation(GLOB_STATE_TELEGRAPH, true);         // Chuyển sang animation báo trước
}

// Hàm thực hiện tấn công
void Glob::attack()
{
    moving = false;                                       // Dừng di chuyển
    frameTimer = 0;                                       // Đặt lại thời gian frame
    slideAmount = 100;                                    // Khoảng cách trượt khi tấn công
    slideAngle = angle;                                   // Góc trượt bằng góc hiện tại
    changeAnimation(GLOB_STATE_ATTACK, true);            // Chuyển sang animation tấn công
}

// Hàm xử lý khi Glob chết
void Glob::die()
{
    moving = false;                                       // Dừng di chuyển
    state = GLOB_STATE_DEAD;                              // Chuyển sang trạng thái đã chết
    changeAnimation(state, true);                         // Chuyển sang animation chết
    // TODO: Thêm âm thanh khi chết cho kẻ địch!

    Glob::globsKilled++;                                  // Tăng số lượng Glob đã bị tiêu diệt
}

// Hàm tìm mục tiêu gần nhất
void Glob::findNearestTarget()
{
    target = NULL;                                        // Đặt lại mục tiêu
    // Duyệt qua danh sách thực thể để tìm mục tiêu
    for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
    {
        if ((*entity)->type == "hero" && (*entity)->active)
        {
            // Nếu đây là hero đầu tiên, đặt làm mục tiêu tạm thời
            if (target == NULL)
            {
                target = (LivingEntity*)(*entity);        // Ép kiểu sang LivingEntity
            }
            else
            {
                // Nếu hero này gần hơn mục tiêu hiện tại
                if (Entity::distanceBetweenTwoEntities(this, (*entity)) < Entity::distanceBetweenTwoEntities(this, target))
                {
                    target = (LivingEntity*)(*entity);    // Cập nhật mục tiêu gần hơn
                }
            }
        }
    }
}

// Hàm thay đổi animation dựa trên trạng thái
void Glob::changeAnimation(int newState, bool resetFrameToBeginning)
{
    state = newState;                                     // Cập nhật trạng thái mới

    if (state == GLOB_STATE_IDLE)
    {
        if (direction == DIR_DOWN)
            currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_DOWN); // Đứng yên hướng xuống
        if (direction == DIR_UP)
            currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_UP);   // Đứng yên hướng lên
        if (direction == DIR_LEFT)
            currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_LEFT); // Đứng yên hướng trái
        if (direction == DIR_RIGHT)
            currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_RIGHT); // Đứng yên hướng phải
    }
    else if (state == GLOB_STATE_MOVE)
    {
        if (direction == DIR_DOWN)
            currentAnim = animSet->getAnimation(GLOB_ANIM_DOWN);      // Di chuyển xuống
        if (direction == DIR_UP)
            currentAnim = animSet->getAnimation(GLOB_ANIM_UP);        // Di chuyển lên
        if (direction == DIR_LEFT)
            currentAnim = animSet->getAnimation(GLOB_ANIM_LEFT);      // Di chuyển trái
        if (direction == DIR_RIGHT)
            currentAnim = animSet->getAnimation(GLOB_ANIM_RIGHT);     // Di chuyển phải
    }
    else if (state == GLOB_STATE_ATTACK)
    {
        if (direction == DIR_DOWN)
            currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_DOWN); // Tấn công xuống
        if (direction == DIR_UP)
            currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_UP);   // Tấn công lên
        if (direction == DIR_LEFT)
            currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_LEFT); // Tấn công trái
        if (direction == DIR_RIGHT)
            currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_RIGHT); // Tấn công phải
    }
    else if (state == GLOB_STATE_TELEGRAPH)
    {
        if (direction == DIR_DOWN)
            currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_DOWN); // Báo trước xuống
        if (direction == DIR_UP)
            currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_UP);   // Báo trước lên
        if (direction == DIR_LEFT)
            currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_LEFT); // Báo trước trái
        if (direction == DIR_RIGHT)
            currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_RIGHT); // Báo trước phải
    }
    else if (state == GLOB_STATE_DEAD)
    {
        currentAnim = animSet->getAnimation(GLOB_ANIM_DIE);           // Animation chết
    }

    if (resetFrameToBeginning)
        currentFrame = currentAnim->getFrame(0);                  // Đặt lại về frame đầu tiên
    else
        currentFrame = currentAnim->getFrame(currentFrame->frameNumber); // Giữ frame hiện tại
}

// Hàm cập nhật animation
void Glob::updateAnimation()
{
    if (currentFrame == NULL || currentAnim == NULL)
        return;                                               // Thoát nếu không có frame hoặc animation

    // Nếu đang ở trạng thái di chuyển nhưng không di chuyển thật
    if (state == GLOB_STATE_MOVE && !moving)
    {
        changeAnimation(GLOB_STATE_IDLE, true);              // Chuyển về đứng yên
    }
    // Nếu đang đứng yên nhưng lại di chuyển
    if (state != GLOB_STATE_MOVE && moving)
    {
        changeAnimation(GLOB_STATE_MOVE, true);              // Chuyển sang di chuyển
    }

    frameTimer += TimeController::timeController.dT;         // Cập nhật thời gian frame
    // Khi hết thời gian của frame hiện tại
    if (frameTimer >= currentFrame->duration)
    {
        // Nếu ở frame cuối của animation
        if (currentFrame->frameNumber == currentAnim->getEndFrameNumber())
        {
            if (state == GLOB_STATE_TELEGRAPH)
            {
                attack();                                    // Sau khi báo trước, tấn công
            }
            else if (state == GLOB_STATE_ATTACK)
            {
                changeAnimation(GLOB_STATE_MOVE, true);     // Sau khi tấn công, chuyển về di chuyển
            }
            else if (state == GLOB_STATE_DEAD)
            {
                frameTimer = 0;                             // Đặt lại thời gian frame
                if (hp > 0)                                 // Nếu hồi sinh
                    changeAnimation(GLOB_STATE_MOVE, true); // Chuyển về di chuyển
                else
                    active = false;                         // Nếu vẫn chết, tắt hoạt động
            }
            else
            {
                currentFrame = currentAnim->getFrame(0);    // Lặp lại animation
            }
        }
        else
        {
            currentFrame = currentAnim->getNextFrame(currentFrame); // Chuyển sang frame tiếp theo
        }
        frameTimer = 0;                                     // Đặt lại thời gian frame
    }
}

// Hàm cập nhật sát thương nhận vào
void Glob::updateDamages()
{
    if (active && hp > 0 && invincibleTimer <= 0)
    {
        for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            if ((*entity)->active && (*entity)->type == "hero")
            {
                LivingEntity* enemy = (LivingEntity*)(*entity); // Ép kiểu sang LivingEntity
                // Nếu bị hero đánh trúng
                if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox))
                {
                    hp -= enemy->damage;                    // Giảm máu

                    if (hp > 0)
                    {
                        // TODO: Phát âm thanh bị thương
                        invincibleTimer = 0.1;              // Thời gian bất tử ngắn
                    }
                    // Tính góc bị đẩy lùi
                    slideAngle = Entity::angleBetweenTwoEntities((*entity), this);
                    slideAmount = 300;                      // Khoảng cách bị đẩy lùi
                }
            }
        }
    }
}