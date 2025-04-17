#include "boss.h"

// Các hằng số cho tên animation
const string Boss::BOSS_ANIM_IDLE = "idle";               // Animation đứng yên
const string Boss::BOSS_ANIM_CHARGE = "charge";           // Animation tích năng lượng
const string Boss::BOSS_ANIM_SHOOT = "shoot";             // Animation bắn
const string Boss::BOSS_ANIM_SLAM = "slam";               // Animation đập
const string Boss::BOSS_ANIM_JUMP_TELEGRAPH = "jumpTelegraph"; // Animation chuẩn bị nhảy
const string Boss::BOSS_ANIM_JUMP = "jump";               // Animation nhảy
const string Boss::BOSS_ANIM_DIE = "die";                 // Animation chết

// Các trạng thái (state)
const int Boss::BOSS_STATE_IDLE = 1;                      // Trạng thái đứng yên
const int Boss::BOSS_STATE_CHARGE = 2;                    // Trạng thái tích năng lượng
const int Boss::BOSS_STATE_SHOOT = 3;                     // Trạng thái bắn
const int Boss::BOSS_STATE_SLAM = 4;                      // Trạng thái đập
const int Boss::BOSS_STATE_JUMP_TELEGRAPH = 5;            // Trạng thái chuẩn bị nhảy
const int Boss::BOSS_STATE_JUMP = 6;                      // Trạng thái nhảy
const int Boss::BOSS_STATE_DEAD = 7;                      // Trạng thái chết

// Các giai đoạn (phase)
const int Boss::BOSS_PHASE_NORMAL = 1;                    // Giai đoạn bình thường
const int Boss::BOSS_PHASE_DAMAGED = 2;                   // Giai đoạn bị thương
const int Boss::BOSS_PHASE_FRANTIC = 3;                   // Giai đoạn hoảng loạn

// Hàm khởi tạo
Boss::Boss(AnimationSet* animSet, AnimationSet* bulletAnimSet)
{
    this->animSet = animSet;
    this->bulletAnimSet = bulletAnimSet;

    type = "enemy"; // Loại thực thể là kẻ thù

    // Vị trí ban đầu
    x = Globals::ScreenWidth / 2;
    y = Globals::ScreenHeight / 2;

    // Các thuộc tính
    moveSpeed = 0;
    moveSpeedMax = 50;
    hp = hpMax = 1000; // Máu tối đa và máu hiện tại
    damage = 0;
    collisionBoxW = 50;
    collisionBoxH = 30;
    collisionBox.w = collisionBoxW;
    collisionBox.h = collisionBoxH;
    collisionBoxYOffset = -14;

    // Bắt đầu với trạng thái đứng yên
    changeAnimation(BOSS_STATE_IDLE, true);

    updateCollisionBox();
}

// Hàm cập nhật trạng thái của Boss
void Boss::update()
{
    // Kiểm tra nếu đã chết
    if (hp < 1 && state != BOSS_STATE_DEAD)
    {
        changeAnimation(BOSS_STATE_DEAD, true);
        moving = false;
        die();
    }

    // AI suy nghĩ hành động tiếp theo
    think();

    // Cập nhật trạng thái bắn
    updateShoot();

    // Cập nhật hộp va chạm
    updateCollisionBox();

    // Cập nhật chuyển động
    updateMovement();

    // Cập nhật va chạm
    updateCollisions();

    // Cập nhật hộp sát thương
    updateHitBox();
    updateDamages();

    // Cập nhật animation
    updateAnimation();

    // Cập nhật thời gian bất tử
    updateInvincibleTimer();
}

// Hàm cập nhật trạng thái bắn
void Boss::updateShoot()
{
    if (state == BOSS_STATE_SHOOT)
    {
        shootTimer -= TimeController::timeController.dT;
        shotTimer -= TimeController::timeController.dT;

        // Nếu hết thời gian bắn, dừng bắn
        if (shootTimer <= 0)
        {
            changeAnimation(BOSS_STATE_IDLE, true);
        }
        else if (shotTimer <= 0) // Nếu vẫn đang bắn và đến lúc bắn
        {
            shotTimer = 0.5;

            // Tạo viên đạn mới
            Bullet* bullet = new Bullet(bulletAnimSet, x, y);
            bullet->angle = angle;
            Entity::entities.push_back(bullet);
        }
    }
}

// Hàm suy nghĩ để quyết định hành động tiếp theo
void Boss::think()
{
    // Tìm mục tiêu gần nhất
    findNearestTarget();

    if (target != NULL) // Nếu có mục tiêu
    {
        if (state == BOSS_STATE_IDLE)
        {
            thinkTimer -= TimeController::timeController.dT;
        }

        // Luôn hướng về phía mục tiêu
        angle = Entity::angleBetweenTwoEntities(this, target);

        // Kiểm tra giai đoạn hiện tại
        if (hp > 250)
        {
            aiState = BOSS_PHASE_NORMAL;
        }
        else if (hp > 100)
        {
            aiState = BOSS_PHASE_DAMAGED;
        }
        else
        {
            aiState = BOSS_PHASE_FRANTIC;
        }

        // Nếu hết thời gian suy nghĩ, chọn hành động mới
        if (thinkTimer <= 0 && state == BOSS_STATE_IDLE)
        {
            frameTimer = 0;

            if (aiState == BOSS_PHASE_NORMAL)
            {
                thinkTimer = 2;
                int action = getRandomNumber(4);
                if (action % 2 == 0)
                {
                    slam();
                }
                else
                {
                    charge();
                }
            }
            else if (aiState == BOSS_PHASE_DAMAGED)
            {
                thinkTimer = 1.5;
                int action = getRandomNumber(6);
                if (action < 2)
                {
                    slam();
                }
                else if (action < 4)
                {
                    charge();
                }
                else
                {
                    jumpTelegraph();
                }
            }
            else
            {
                thinkTimer = 1;
                int action = getRandomNumber(4);
                if (action % 2 == 0)
                {
                    jumpTelegraph();
                }
                else
                {
                    charge();
                }
            }
        }
    }
    else
    {
        moving = 0;
        changeAnimation(BOSS_STATE_IDLE, (state != BOSS_STATE_IDLE));
    }
}

// Các hành động của Boss
void Boss::charge()
{
    moving = false;
    changeAnimation(BOSS_STATE_CHARGE, true);
}

void Boss::shoot()
{
    moving = false;
    shootTimer = (aiState != BOSS_PHASE_NORMAL) ? 3 : 5;
    shotTimer = 0;
    changeAnimation(BOSS_STATE_SHOOT, true);
}

void Boss::slam()
{
    moving = false;
    changeAnimation(BOSS_STATE_SLAM, true);
}

void Boss::jumpTelegraph()
{
    moving = false;
    frameTimer = 0;
    changeAnimation(BOSS_STATE_JUMP_TELEGRAPH, true);
}

void Boss::jump()
{
    moving = false;
    frameTimer = 0;
    slideAmount = 100;
    slideAngle = angle;
    changeAnimation(BOSS_STATE_JUMP, true);
}

void Boss::die()
{
    moving = false;
    changeAnimation(BOSS_STATE_DEAD, true);
    SoundManager::soundManager.playSound("enemyDie");
    bossesKilled++;
}

// Tìm mục tiêu gần nhất
void Boss::findNearestTarget()
{
    target = NULL;
    for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
    {
        if ((*entity)->type == "hero" && (*entity)->active)
        {
            if (target == NULL || Entity::distanceBetweenTwoEntities(this, (*entity)) < Entity::distanceBetweenTwoEntities(this, target))
            {
                target = (LivingEntity*)(*entity);
            }
        }
    }
}

// Thay đổi animation
void Boss::changeAnimation(int newState, bool resetFrameToBeginning)
{
    state = newState;

    if (state == BOSS_STATE_IDLE)
    {
        currentAnim = animSet->getAnimation(BOSS_ANIM_IDLE);
    }
    else if (state == BOSS_STATE_CHARGE)
    {
        currentAnim = animSet->getAnimation(BOSS_ANIM_CHARGE);
    }
    else if (state == BOSS_STATE_SHOOT)
    {
        currentAnim = animSet->getAnimation(BOSS_ANIM_SHOOT);
    }
    else if (state == BOSS_STATE_SLAM)
    {
        currentAnim = animSet->getAnimation(BOSS_ANIM_SLAM);
    }
    else if (state == BOSS_STATE_JUMP_TELEGRAPH)
    {
        currentAnim = animSet->getAnimation(BOSS_ANIM_JUMP_TELEGRAPH);
    }
    else if (state == BOSS_STATE_JUMP)
    {
        currentAnim = animSet->getAnimation(BOSS_ANIM_JUMP);
    }
    else if (state == BOSS_STATE_DEAD)
    {
        currentAnim = animSet->getAnimation(BOSS_ANIM_DIE);
    }

    currentFrame = resetFrameToBeginning ? currentAnim->getFrame(0) : currentAnim->getFrame(currentFrame->frameNumber);
}

// Cập nhật animation
void Boss::updateAnimation()
{
    if (currentFrame == NULL || currentAnim == NULL)
    {
        return;
    }

    frameTimer += TimeController::timeController.dT;

    if (frameTimer >= currentFrame->duration)
    {
        if (currentFrame->frameNumber == currentAnim->getEndFrameNumber())
        {
            if (state == BOSS_STATE_CHARGE)
            {
                shoot();
            }
            else if (state == BOSS_STATE_JUMP_TELEGRAPH)
            {
                jump();
            }
            else if (state == BOSS_STATE_SLAM || state == BOSS_STATE_JUMP)
            {
                changeAnimation(BOSS_STATE_IDLE, true);
            }
            else if (state == BOSS_STATE_DEAD)
            {
                frameTimer = 0;
                if (hp > 0)
                {
                    changeAnimation(BOSS_STATE_IDLE, true);
                }
                else
                {
                    active = false;
                }
            }
        }
        else
        {
            currentFrame = currentAnim->getNextFrame(currentFrame);
        }
        frameTimer = 0;
    }
}

// Cập nhật sát thương
void Boss::updateDamages()
{
    if (active && hp > 0 && invincibleTimer <= 0)
    {
        for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            if ((*entity)->active && (*entity)->type == "hero")
            {
                LivingEntity* enemy = (LivingEntity*)(*entity);
                if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox))
                {
                    enemy->hitLanded(this);
                    hp -= enemy->damage;

                    if (hp > 0)
                    {
                        SoundManager::soundManager.playSound("enemyHit");
                        invincibleTimer = 0.1;
                    }
                }
            }
        }
    }
}

int Boss::bossesKilled = 0; // Số lượng Boss đã bị tiêu diệt