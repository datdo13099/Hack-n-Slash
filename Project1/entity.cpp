#include "entity.h"

const int Entity::DIR_UP = 0, Entity::DIR_DOWN = 1, Entity::DIR_LEFT = 2, Entity::DIR_RIGHT = 3, Entity::DIR_NONE = -1;

// CÁC HÀM ẢO
void Entity::update() { ; } // phương thức cập nhật entity, sẽ được override lại

void Entity::draw() {
    // phương thức vẽ entity, có thể override nếu muốn vẽ cái gì đó cụ thể hơn

    // vẽ frame hiện tại của entity
    if (currentFrame != NULL && active) {
        currentFrame->Draw(animSet->spriteSheet, x, y);
    }

    // vẽ hộp va chạm (collisionBox)
    if (solid && Globals::debugging) {
        // đặt màu vẽ hiện tại (không ảnh hưởng đến texture)
        SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(Globals::renderer, &collisionBox);
    }
}
    

void Entity::move(float angle)
{
    moving = true;
    moveSpeed = moveSpeedMax;
    this->angle = angle;

    int newDirection = angleToDirection(angle);
    //nếu hướng thay đổi, hãy cập nhật hoạt ảnh hiện tại
    if (direction != newDirection)
    {
        direction = newDirection;
        changeAnimation(state, false);
    }
}


void Entity::updateMovement() 
{
    updateCollisionBox();
    // lưu collisionBox trước khi di chuyển
    lastCollisionBox = collisionBox;

    // đặt lại tổng khoảng di chuyển
    totalXMove = 0;
    totalYMove = 0;

    if (moveSpeed > 0) {
        // tính khoảng cách di chuyển dựa trên tốc độ, dt (thời gian từ vòng lặp trước) và nhân với hệ số nội suy
        float moveDist = moveSpeed * (TimeController::timeController.dT) * moveLerp;
        if (moveDist > 0)
        {
            // xMove = khoảng cách * cos(góc tính theo radian)
            float xMove = moveDist * (cos(angle * Globals::PI / 180));
            // yMove = khoảng cách * sin(góc tính theo radian)
            float yMove = moveDist * (sin(angle * Globals::PI / 180));

            x += xMove;
            y += yMove;

            totalXMove = xMove;
            totalYMove = yMove;

            if (!moving)
            {
                moveSpeed -= moveDist;
            }
        }
    }
    //sliding around
    if (slideAmount > 0)
    {
        float slideDist = slideAmount * TimeController::timeController.dT * moveLerp;
        if (slideDist > 0)
        {
            float xMove = slideDist * (cos(slideAngle * Globals::PI / 180));
            float yMove = slideDist * (sin(slideAngle * Globals::PI / 180));

            x += xMove;
            y += yMove;

            totalXMove = xMove;
            totalYMove = yMove;
            slideAmount -= slideDist;
        }
        else
        {
            slideAmount = 0;
        }
    }
    // sau khi đã di chuyển, cập nhật collisionBox đến vị trí hiện tại
    updateCollisionBox();
    // để hỗ trợ kiểm tra va chạm, hợp nhất collisionBox với lastCollisionBox
    SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);
}


void Entity::updateCollisionBox()
{
    collisionBox.x = x - collisionBox.w / 2;
    collisionBox.y = y + collisionBoxYOffset;
    collisionBox.w = collisionBoxW;
    collisionBox.h = collisionBoxH;
}

//virtual void changeAnimation(int newState, bool resetFrameToBeginning) = 0; // thay đổi animation theo trạng thái mới, có thể reset frame về ban đầu
void Entity::updateCollisions() 
{
    if (active && collideWithSolids && (moveSpeed > 0 || slideAmount > 0)) 
    {

        // danh sách các va chạm tiềm năng
        list<Entity*> collisions;

        // duyệt qua danh sách thực thể
        for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) 
        {

            // nếu thực thể này va chạm với thực thể hiện tại (dựa trên collisionBox hợp nhất)
            if ((*entity)->active
                && (*entity)->type != this->type
                && (*entity)->solid
                && Entity::checkCollision(collisionBox, (*entity)->collisionBox)) 
            {
                // thêm vào danh sách va chạm
                collisions.push_back(*entity);
            }
        }
        // nếu chúng ta có một danh sách các thực thể tiềm năng mà ta có thể va chạm, thì hãy kiểm tra chúng đúng cách để thực hiện xử lý va chạm
        if (collisions.size() > 0) 
        {
            updateCollisionBox();
            // kiểm tra va chạm nhiều mẫu từ vị trí bắt đầu đến vị trí chúng ta dự định đi đến

            // đầu tiên, ta sẽ tính khoảng cách mẫu cần di chuyển giữa các lần kiểm tra
            float boxTravelSize = 0;
            if (collisionBox.w < collisionBox.h)
                boxTravelSize = collisionBox.w / 4;
            else
                boxTravelSize = collisionBox.h / 4;

            // sử dụng sampleBox để kiểm tra va chạm từ điểm bắt đầu đến điểm kết thúc, 
            // di chuyển với khoảng cách boxTravelSize mỗi lần lấy mẫu
            SDL_Rect sampleBox = lastCollisionBox;
            float movementAngle = Entity::angleBetweenTwoRects(lastCollisionBox, collisionBox);

            bool foundCollision = false;
            while (!foundCollision) 
            {
                // kiểm tra va chạm tại vị trí hiện tại của sampleBox
                SDL_Rect sampleBox = lastCollisionBox;
                for (auto entity = collisions.begin(); entity != collisions.end(); entity++)
                {
                    if (SDL_IntersectRect(&sampleBox, &(*entity)->collisionBox, &intersection))
                    {
                        foundCollision = true;  // phát hiện va chạm
                        moveSpeed = 0;          // dừng di chuyển
                        moving = false;         // trạng thái không di chuyển
                        slideAngle = angleBetweenTwoEntities((*entity), this); // tính góc trượt khi va chạm

                        // hiện tại đang va chạm với một entity, cần xử lý va chạm
                        if (intersection.w < intersection.h) 
                        {
                            if (lastCollisionBox.x + lastCollisionBox.w / 2 < (*entity)->collisionBox.x + (*entity)->collisionBox.w / 2)
                                sampleBox.x -= intersection.w; // bắt đầu từ bên trái, di chuyển sang trái để thoát khỏi va chạm
                            else
                                sampleBox.x += intersection.w; // ngược lại, bắt đầu từ bên phải
                        }
                        else 
                        {
                            if (lastCollisionBox.y + lastCollisionBox.h / 2 < (*entity)->collisionBox.y + (*entity)->collisionBox.h / 2)
                                sampleBox.y -= intersection.h; // bắt đầu từ trên, di chuyển lên trên để thoát khỏi va chạm
                            else
                                sampleBox.y += intersection.h; // ngược lại, bắt đầu từ dưới
                        }
                    }
                }
                // nếu phát hiện va chạm hoặc sampleBox đến cùng vị trí với collisionBox, thoát vòng lặp

                // di chuyển sampleBox lên để kiểm tra vị trí tiếp theo
            }
        }
    }
}


// CÁC HÀM HỖ TRỢ
static float distanceBetweenTwoRects(SDL_Rect& r1, SDL_Rect& r2); // tính khoảng cách giữa hai hình chữ nhật
static float distanceBetweenTwoEntities(Entity* e1, Entity* e2); // tính khoảng cách giữa hai entity
static float angleBetweenTwoEntities(Entity* e1, Entity* e2); // tính góc giữa hai entity
static bool checkCollision(SDL_Rect cbox1, SDL_Rect cbox2); // kiểm tra va chạm giữa hai hộp va chạm
static int angleToDirection(float angle); // chuyển góc thành hướng di chuyển
static float angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2); // tính góc giữa hai điểm trong không gian 2D
static float angleBetweenTwoRects(SDL_Rect& r1, SDL_Rect& r2); // tính góc giữa hai hình chữ nhật

// danh sách entity toàn cục có thể tham chiếu bất cứ lúc nào
static list<Entity*> entities;
static bool EntityCompare(const Entity* const& a, const Entity* const& b); // so sánh hai entity để hỗ trợ sắp xếp danh sách
static void removeInactiveEntitiesFromList(list<Entity*>* entityList, bool deleteEntities); // xóa các entity không hoạt động khỏi danh sách
static void removeAllFromList(list<Entity*>* entityList, bool deleteEntities); // xóa tất cả entity khỏi danh sách (có thể xóa hoàn toàn khỏi bộ nhớ)

