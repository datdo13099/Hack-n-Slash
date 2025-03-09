#include "livingEntity.h"

void LivingEntity::updateHitBox()
{
    // giả định sát thương là 0 ban đầu
    damage = 0;

    GroupBox* hitBoxes = (GroupBox*)GroupBuilder::findGroupByName("hitBox", currentFrame->frameData);
    if (hitBoxes != NULL && hitBoxes->numberOfDataInGroup() > 0)
    {
        // cập nhật hitbox
        SDL_Rect hb = hitBoxes->data.front();
        hitBox.x = x - currentFrame->offSet.x + hb.x;
        hitBox.y = y - currentFrame->offSet.y + hb.y;
        hitBox.w = hb.w;
        hitBox.h = hb.h;

        // cập nhật sát thương
        GroupNumber* damages = (GroupNumber*)GroupBuilder::findGroupByName("damage", currentFrame->frameData);
        if (damages != NULL && damages->numberOfDataInGroup() > 0)
        {
            damage = damages->data.front();
        }
    }
}

void LivingEntity::updateInvincibleTimer()
{
    if (invincibleTimer > 0)
    {
        invincibleTimer -= TimeController::timeController.dT;
    }
}

void LivingEntity::draw()
{
    // kiểm tra nếu có frame hiện tại và thực thể đang hoạt động
    if (currentFrame != NULL && active)
    {
        // nếu đang trong trạng thái bất tử và có sprite trắng, vẽ bằng sprite trắng
        if (invincibleTimer > 0 && animSet->whiteSpriteSheet != NULL)
            currentFrame->Draw(animSet->whiteSpriteSheet, x, y);
        else // nếu không thì vẽ bằng sprite thường
            currentFrame->Draw(animSet->spriteSheet, x, y);
    }
    // vẽ hộp va chạm
    if (solid && Globals::debugging)
    {
        // đặt màu vẽ hiện tại (không ảnh hưởng đến texture và các yếu tố khác)
        SDL_SetRenderDrawColor(Globals::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(Globals::renderer, &lastCollisionBox);

        SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(Globals::renderer, &collisionBox);

        SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(Globals::renderer, &hitBox);
    }
}