#include "grob.h"

const string Grob::GROB_ANIM_UP = "up";
const string Grob::GROB_ANIM_DOWN = "down";
const string Grob::GROB_ANIM_LEFT = "left";
const string Grob::GROB_ANIM_RIGHT = "right";
const string Grob::GROB_ANIM_IDLE_UP = "idleUp";
const string Grob::GROB_ANIM_IDLE_DOWN = "idleDown";
const string Grob::GROB_ANIM_IDLE_LEFT = "idleLeft";
const string Grob::GROB_ANIM_IDLE_RIGHT = "idleRight";
const string Grob::GROB_ANIM_DIE = "die";

const int Grob::GROB_STATE_IDLE = 0;
const int Grob::GROB_STATE_MOVE = 1;
const int Grob::GROB_STATE_DEAD = 2;

int Grob::grobsKilled = 0;

// Hàm khởi tạo chính
Grob::Grob(AnimationSet* animSet)
{
    this->animSet = animSet;

    type = "enemy";

    // Thiết lập giá trị mặc định
    x = Globals::ScreenWidth / 2;
    y = Globals::ScreenHeight / 2;
    moveSpeed = 0;
    moveSpeedMax = 20;
    hp = hpMax = 1;
    damage = 0;
    collisionBoxW = 18;
    collisionBoxH = 16;
    collisionBox.w = collisionBoxW;
    collisionBox.h = collisionBoxH;

    collisionBoxYOffset = -14;

    direction = DIR_DOWN;

    changeAnimation(GROB_STATE_IDLE, true);

    updateCollisionBox();
}

void Grob::update()
{
    // Kiểm tra nếu đã chết
    if (hp < 1 && state != GROB_STATE_DEAD)
    {
        changeAnimation(GROB_STATE_DEAD, true);
        moving = false;
        die();
    }

    think();

    // Cập nhật hộp va chạm
    updateCollisionBox();
    // Cập nhật chuyển động/đầu vào
    updateMovement();

    // Xử lý va chạm
    updateCollisions();

    // Cập nhật hộp sát thương sau khi đã xác định vị trí cuối cùng
    updateHitBox();
    updateDamages();

    // Cập nhật hoạt hình
    updateAnimation();
    // Cập nhật bộ đếm thời gian và các thứ khác
    updateInvincibleTimer();
}

void Grob::think()
{
    if (state == GROB_STATE_IDLE || state == GROB_STATE_MOVE)
    {
        thinkTimer -= TimeController::timeController.dT;

        if (thinkTimer <= 0)
        {
            // Đặt lại bộ đếm thời gian
            thinkTimer = 0.5;
            int action = rand() % 10;
            // Ở trạng thái nghỉ
            if (action < 3)
            {
                // Di chuyển ngẫu nhiên theo một góc
                angle = rand() % 360;
                moving = true;
                changeAnimation(GROB_STATE_MOVE, state != GROB_STATE_MOVE);
            }
            else
            {
                // Chỉ nghỉ ngơi vui vẻ
                moving = false;
                changeAnimation(GROB_STATE_IDLE, true);
            }
        }
    }

    if (state == GROB_STATE_MOVE)
    {
        move(angle);
    }
}

void Grob::die()
{
    moving = false;
    state = GROB_STATE_DEAD;
    changeAnimation(state, true);
    SoundManager::soundManager.playSound("enemyDie");

    // Tăng số lượng kẻ địch bị tiêu diệt
    Grob::grobsKilled++;
}

void Grob::changeAnimation(int newState, bool resetFrameToBegging)
{
    state = newState;
    if (state == GROB_STATE_IDLE)
    {
        if (direction == DIR_DOWN)
        {
            currentAnim = animSet->getAnimation(GROB_ANIM_IDLE_DOWN);
        }
        if (direction == DIR_UP)
        {
            currentAnim = animSet->getAnimation(GROB_ANIM_IDLE_UP);
        }
        if (direction == DIR_LEFT)
        {
            currentAnim = animSet->getAnimation(GROB_ANIM_IDLE_LEFT);
        }
        if (direction == DIR_RIGHT)
        {
            currentAnim = animSet->getAnimation(GROB_ANIM_IDLE_RIGHT);
        }
    }
    else if (state == GROB_STATE_MOVE)
    {
        if (direction == DIR_DOWN)
        {
            currentAnim = animSet->getAnimation(GROB_ANIM_DOWN);
        }
        if (direction == DIR_UP)
        {
            currentAnim = animSet->getAnimation(GROB_ANIM_UP);
        }
        if (direction == DIR_LEFT)
        {
            currentAnim = animSet->getAnimation(GROB_ANIM_LEFT);
        }
        if (direction == DIR_RIGHT)
        {
            currentAnim = animSet->getAnimation(GROB_ANIM_RIGHT);
        }
    }
    else if (state == GROB_STATE_DEAD)
    {
        // Luôn đối mặt với người xem
        currentAnim = animSet->getAnimation(GROB_ANIM_DIE);
    }

    if (resetFrameToBegging)
    {
        currentFrame = currentAnim->getFrame(0);
    }
    else
    {
        currentFrame = currentAnim->getFrame(currentFrame->frameNumber); // Thay đổi hướng nhưng giữ nguyên frame hiện tại
    }
}

void Grob::updateAnimation()
{
    if (currentFrame == NULL || currentAnim == NULL)
    {
        return; // Không làm gì nếu không có frame hoặc hoạt hình
    }

    // Nếu đang ở trạng thái di chuyển nhưng không di chuyển thật, quay lại trạng thái nghỉ
    if (state == GROB_STATE_MOVE && !moving)
    {
        changeAnimation(GROB_STATE_IDLE, true);
    }
    // Nếu đang di chuyển nhưng không ở trạng thái di chuyển, chuyển sang trạng thái di chuyển
    if (state != GROB_STATE_MOVE && moving)
    {
        changeAnimation(GROB_STATE_MOVE, true);
    }

    frameTimer += TimeController::timeController.dT;
    // Đã đến lúc thay đổi frame
    if (frameTimer >= currentFrame->duration)
    {
        // Nếu ở cuối hoạt hình
        if (currentFrame->frameNumber == currentAnim->getEndFrameNumber())
        {
            if (state == GROB_STATE_DEAD)
            {
                frameTimer = 0;
                // Nếu hồi sinh, chuyển về trạng thái di chuyển
                if (hp > 0)
                {
                    changeAnimation(GROB_STATE_MOVE, true);
                }
                else
                {
                    active = false;
                }
            }
            else
            {
                // Chỉ đặt lại hoạt hình
                currentFrame = currentAnim->getFrame(0);
            }
        }
        else
        {
            // Chuyển sang frame tiếp theo
            currentFrame = currentAnim->getNextFrame(currentFrame);
        }
        frameTimer = 0; // Bước quan trọng, nếu bỏ qua frame tiếp theo sẽ bị bỏ qua nhanh
    }
}

void Grob::updateHitBox()
{
    // Grob là một hộp gây đau liên tục khi di chuyển
    hitBox.w = collisionBox.w + 2;
    hitBox.h = collisionBox.h + 2;
    hitBox.x = collisionBox.x - 1;
    hitBox.y = collisionBox.y - 1;
    if (hp > 0)
    {
        damage = 10;
    }
    else
    {
        damage = 0;
    }
}

void Grob::updateDamages()
{
    if (active && hp > 0 && invincibleTimer <= 0)
    {
        for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            if ((*entity)->active && (*entity)->type == "hero")
            {
                // Tham chiếu như một LivingEntity để truy cập hộp sát thương của kẻ địch
                LivingEntity* enemy = (LivingEntity*)(*entity);

                if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox))
                {
                    hp -= enemy->damage;

                    if (hp > 0)
                    {
                        SoundManager::soundManager.playSound("enemyHit");
                        invincibleTimer = 0.1;
                    }
                    // Góc từ thực thể khác đến thực thể này
                    slideAngle = Entity::angleBetweenTwoEntities((*entity), this);
                    slideAmount = 300;
                }
            }
        }
    }
}