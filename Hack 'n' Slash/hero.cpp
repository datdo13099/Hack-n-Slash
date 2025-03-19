#include "hero.h"

// Định nghĩa các hằng số tĩnh cho tên animation của Hero
const string Hero::HERO_ANIM_UP = "up";              // Animation di chuyển lên
const string Hero::HERO_ANIM_DOWN = "down";          // Animation di chuyển xuống
const string Hero::HERO_ANIM_LEFT = "left";          // Animation di chuyển sang trái
const string Hero::HERO_ANIM_RIGHT = "right";        // Animation di chuyển sang phải
const string Hero::HERO_ANIM_IDLE_UP = "idleUp";     // Animation đứng yên hướng lên
const string Hero::HERO_ANIM_IDLE_DOWN = "idleDown"; // Animation đứng yên hướng xuống
const string Hero::HERO_ANIM_IDLE_LEFT = "idleLeft"; // Animation đứng yên hướng trái
const string Hero::HERO_ANIM_IDLE_RIGHT = "idleRight"; // Animation đứng yên hướng phải
const string Hero::HERO_SLASH_ANIM_UP = "slashUp";   // Animation tấn công hướng lên
const string Hero::HERO_SLASH_ANIM_DOWN = "slashDown"; // Animation tấn công hướng xuống
const string Hero::HERO_SLASH_ANIM_LEFT = "slashLeft"; // Animation tấn công hướng trái
const string Hero::HERO_SLASH_ANIM_RIGHT = "slashRight"; // Animation tấn công hướng phải
const string Hero::HERO_DASH_ANIM_UP = "dashUp";     // Animation lướt nhanh hướng lên
const string Hero::HERO_DASH_ANIM_DOWN = "dashDown"; // Animation lướt nhanh hướng xuống
const string Hero::HERO_DASH_ANIM_LEFT = "dashLeft"; // Animation lướt nhanh hướng trái
const string Hero::HERO_DASH_ANIM_RIGHT = "dashRight"; // Animation lướt nhanh hướng phải
const string Hero::HERO_ANIM_DIE = "die";            // Animation khi chết

// Định nghĩa các hằng số tĩnh cho trạng thái của Hero
const int Hero::HERO_STATE_IDLE = 0;  // Trạng thái đứng yên
const int Hero::HERO_STATE_MOVE = 1;  // Trạng thái di chuyển
const int Hero::HERO_STATE_SLASH = 2; // Trạng thái tấn công
const int Hero::HERO_STATE_DASH = 3;  // Trạng thái lướt nhanh
const int Hero::HERO_STATE_DEAD = 4;  // Trạng thái đã chết

// Hàm khởi tạo Hero
Hero::Hero(AnimationSet* animSet)
{
    this->animSet = animSet;  // Gán tập hợp animation cho Hero
    type = "hero";            // Đặt loại thực thể là "hero"

    // Thiết lập các giá trị mặc định cho Hero
    x = Globals::ScreenWidth / 2;  // Đặt vị trí x ở giữa màn hình
    y = Globals::ScreenHeight / 2; // Đặt vị trí y ở giữa màn hình
    moveSpeed = 0;                 // Tốc độ di chuyển ban đầu là 0
    moveSpeedMax = 80;             // Tốc độ tối đa là 80
    hp = hpMax = 200;               // Máu hiện tại và tối đa là 200
    damage = 0;                    // Sát thương mặc định là 0
    collisionBoxW = 20;            // Chiều rộng hộp va chạm
    collisionBoxH = 24;            // Chiều cao hộp va chạm
    collisionBoxYOffset = -20;     // Độ lệch hộp va chạm theo trục y

    direction = DIR_DOWN;          // Hướng mặc định là xuống dưới

    changeAnimation(HERO_STATE_IDLE, true);  // Đặt animation ban đầu là đứng yên
    updateCollisionBox();                    // Cập nhật hộp va chạm
}

// Hàm cập nhật trạng thái của Hero mỗi frame
void Hero::update()
{
    // Kiểm tra nếu đã chết
    if (hp < 1 && state != HERO_STATE_DEAD)
    {
        changeAnimation(HERO_STATE_DEAD, true);  // Chuyển sang trạng thái chết
        moving = false;                          // Ngừng di chuyển
        die();                                   // Gọi hàm xử lý khi chết
    }

    updateCollisionBox();   // Cập nhật hộp va chạm
    updateMovement();       // Cập nhật chuyển động
    updateCollisions();     // Cập nhật va chạm với các thực thể khác

    updateHitBox();         // Cập nhật hộp gây sát thương
    updateDamages();        // Cập nhật sát thương nhận vào

    updateAnimation();      // Cập nhật animation
    updateInvincibleTimer(); // Cập nhật thời gian bất tử
}

// Hàm thực hiện hành động tấn công
void Hero::slash() 
{
    if (hp > 0 && (state == HERO_STATE_MOVE || state == HERO_STATE_IDLE)) 
    {
        moving = false;                          // Ngừng di chuyển
        frameTimer = 0;                          // Đặt lại bộ đếm khung hình
        changeAnimation(HERO_STATE_SLASH, true); // Chuyển sang animation tấn công
        SoundManager::soundManager.playSound("swing");
    }
}

// Hàm thực hiện hành động lướt nhanh
void Hero::dash()
{
    if (hp > 0 && (state == HERO_STATE_MOVE || state == HERO_STATE_IDLE))
    {
        moving = false;                          // Ngừng di chuyển
        frameTimer = 0;
        slideAngle = angle;
        slideAmount = 300;
        // Chỉ đặt invincibleTimer nếu hero chưa bất tử
        if (invincibleTimer < 999999)
        {
            invincibleTimer = 0.1;
        }
        changeAnimation(HERO_STATE_DASH, true);  // Chuyển sang animation lướt nhanh
        SoundManager::soundManager.playSound("dash");
    }
}

// Hàm xử lý khi Hero chết
void Hero::die()
{
    moving = false;                          // Ngừng di chuyển
    changeAnimation(HERO_STATE_DEAD, true);  // Chuyển sang animation chết
}

// Hàm hồi sinh Hero
void Hero::revive()
{
    hp = hpMax;                              // Khôi phục máu tối đa
    changeAnimation(HERO_STATE_IDLE, true);  // Đặt lại trạng thái đứng yên
    moving = false;                          // Ngừng di chuyển
    x = Globals::ScreenWidth / 2;            // Đặt lại vị trí giữa màn hình
    y = Globals::ScreenHeight / 2;           // Đặt lại vị trí giữa màn hình
    slideAmount = 0;                         // Đặt lại lượng trượt về 0
}

// Hàm thay đổi animation dựa trên trạng thái mới
void Hero::changeAnimation(int newState, bool resetFrameToBeginning)
{
    state = newState;  // Cập nhật trạng thái mới

    // Chọn animation dựa trên trạng thái và hướng
    if (state == HERO_STATE_IDLE)
    {
        if (direction == DIR_DOWN)
            currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_DOWN);
        else if (direction == DIR_UP)
            currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_UP);
        else if (direction == DIR_LEFT)
            currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_LEFT);
        else if (direction == DIR_RIGHT)
            currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_RIGHT);
    }
    else if (state == HERO_STATE_MOVE)
    {
        if (direction == DIR_DOWN)
            currentAnim = animSet->getAnimation(HERO_ANIM_DOWN);
        else if (direction == DIR_UP)
            currentAnim = animSet->getAnimation(HERO_ANIM_UP);
        else if (direction == DIR_LEFT)
            currentAnim = animSet->getAnimation(HERO_ANIM_LEFT);
        else if (direction == DIR_RIGHT)
            currentAnim = animSet->getAnimation(HERO_ANIM_RIGHT);
    }
    else if (state == HERO_STATE_SLASH)
    {
        if (direction == DIR_DOWN)
            currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_DOWN);
        else if (direction == DIR_UP)
            currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_UP);
        else if (direction == DIR_LEFT)
            currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_LEFT);
        else if (direction == DIR_RIGHT)
            currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_RIGHT);
    }
    else if (state == HERO_STATE_DASH)
    {
        if (direction == DIR_DOWN)
            currentAnim = animSet->getAnimation(HERO_DASH_ANIM_DOWN);
        else if (direction == DIR_UP)
            currentAnim = animSet->getAnimation(HERO_DASH_ANIM_UP);
        else if (direction == DIR_LEFT)
            currentAnim = animSet->getAnimation(HERO_DASH_ANIM_LEFT);
        else if (direction == DIR_RIGHT)
            currentAnim = animSet->getAnimation(HERO_DASH_ANIM_RIGHT);
    }
    else if (state == HERO_STATE_DEAD)
    {
        currentAnim = animSet->getAnimation(HERO_ANIM_DIE);
    }

    // Đặt lại khung hình nếu cần
    if (resetFrameToBeginning)
        currentFrame = currentAnim->getFrame(0);  // Bắt đầu từ khung đầu tiên
    else
        currentFrame = currentAnim->getFrame(currentFrame->frameNumber);  // Giữ khung hiện tại
}

void Hero::updateAnimation() 
{
    if (currentFrame == NULL || currentAnim == NULL)
        return;  // Không thể xử lý animation nếu thiếu con trỏ

    if (state == HERO_STATE_MOVE && !moving) 
    {
        changeAnimation(HERO_STATE_IDLE, true);
    }
    if (state != HERO_STATE_MOVE && moving) 
    {
        changeAnimation(HERO_STATE_MOVE, true);
    }

    frameTimer += TimeController::timeController.dT;  // Cộng thêm thời gian delta
    // Nếu đã đến lúc chuyển khung hình
    if (frameTimer >= currentFrame->duration) 
    {
        if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()) 
        {
            if (state == HERO_STATE_SLASH || state == HERO_STATE_DASH) 
            {
                changeAnimation(HERO_STATE_MOVE, true);
            }
            else if (state == HERO_STATE_DEAD && hp > 0)
            {
                // Nếu đã chết nhưng máu hồi phục, đứng dậy
                changeAnimation(HERO_STATE_MOVE, true);
            }
            else 
            {
                currentFrame = currentAnim->getFrame(0);
            }
        }
        else 
        {
            currentFrame = currentAnim->getNextFrame(currentFrame);
        }
        frameTimer = 0;  // Đặt lại bộ đếm khung hình
    }
}

// Hàm cập nhật sát thương nhận vào
void Hero::updateDamages()
{
    if (active && hp > 0 && invincibleTimer <= 0)  // Chỉ nhận sát thương khi còn sống và không bất tử
    {
        for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            if ((*entity)->active && (*entity)->type == "enemy")  // Kiểm tra nếu là kẻ địch
            {
                // Ép kiểu về LivingEntity vì kẻ địch cũng là thực thể sống
                LivingEntity* enemy = (LivingEntity*)(*entity);

                // Nếu kẻ địch gây sát thương và hộp va chạm giao với hitBox
                if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox))
                {
                    hp -= enemy->damage;  // Giảm máu theo sát thương của kẻ địch

                    // Nếu vẫn còn sống
                    if (hp > 0)
                    {
                        invincibleTimer = 0.3;  // Đặt thời gian bất tử là 0.3 giây
                        SoundManager::soundManager.playSound("hit");
                    }

                    // Tính góc và lực đẩy khi bị trúng
                    slideAngle = Entity::angleBetweenTwoEntities((*entity), this);
                    slideAmount = 200;  // Lực đẩy là 200 đơn vị
                }
            }
        }
    }
}