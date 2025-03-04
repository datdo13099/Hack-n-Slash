#include "entity.h"

// Định nghĩa các hằng số hướng di chuyển
const int Entity::DIR_UP = 0, Entity::DIR_DOWN = 1, Entity::DIR_LEFT = 2, Entity::DIR_RIGHT = 3, Entity::DIR_NONE = -1;

// Hàm cập nhật trạng thái thực thể (mặc định không làm gì)
void Entity::update()
{
    ;  // Ghi đè hàm này trong lớp con để thực hiện các hành động cụ thể
}

// Hàm vẽ thực thể lên màn hình
void Entity::draw()
{
    // Ghi đè hàm này nếu muốn thay đổi cách vẽ
    // Vẽ khung hình hiện tại nếu có
    if (currentFrame != NULL && active)
    {
        currentFrame->Draw(animSet->spriteSheet, x, y);  // Vẽ khung hình từ spriteSheet tại vị trí x, y
    }

    // Vẽ hộp va chạm nếu là thực thể rắn và đang ở chế độ debug
    if (solid && Globals::debugging)
    {
        SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);  // Đặt màu đỏ để vẽ hộp
        SDL_RenderDrawRect(Globals::renderer, &collisionBox);  // Vẽ khung hộp va chạm
    }
}

// Hàm di chuyển thực thể theo góc
void Entity::move(float angle)
{
    moving = true;          // Đặt trạng thái di chuyển
    moveSpeed = moveSpeedMax;  // Đặt tốc độ tối đa
    this->angle = angle;    // Cập nhật góc di chuyển

    int newDirection = angleToDirection(angle);  // Chuyển góc thành hướng
    // Nếu hướng thay đổi, cập nhật animation
    if (direction != newDirection)
    {
        direction = newDirection;
        changeAnimation(state, false);  // Thay đổi animation nhưng không đặt lại khung hình
    }
}

// Hàm cập nhật chuyển động của thực thể
void Entity::updateMovement()
{
    updateCollisionBox();  // Cập nhật hộp va chạm trước khi di chuyển
    lastCollisionBox = collisionBox;  // Lưu lại hộp va chạm trước đó

    totalXMove = 0;  // Đặt lại tổng quãng đường di chuyển theo x
    totalYMove = 0;  // Đặt lại tổng quãng đường di chuyển theo y

    if (moveSpeed > 0)  // Nếu đang di chuyển
    {
        float moveDist = moveSpeed * (TimeController::timeController.dT) * moveLerp;  // Tính quãng đường dựa trên tốc độ và thời gian
        if (moveDist > 0)
        {
            float xMove = moveDist * (cos(angle * Globals::PI / 180));  // Tính quãng đường theo x (cos - radian)
            float yMove = moveDist * (sin(angle * Globals::PI / 180));  // Tính quãng đường theo y (sin - radian)

            x += xMove;  // Cập nhật vị trí x
            y += yMove;  // Cập nhật vị trí y

            totalXMove = xMove;  // Lưu tổng quãng đường x
            totalYMove = yMove;  // Lưu tổng quãng đường y

            if (!moving)
                moveSpeed -= moveDist;  // Giảm tốc độ nếu không còn di chuyển chủ động
        }
    }

    // Xử lý trượt (slide)
    if (slideAmount > 0)
    {
        float slideDist = slideAmount * TimeController::timeController.dT * moveLerp;  // Tính quãng đường trượt
        if (slideDist > 0)
        {
            float xMove = slideDist * (cos(slideAngle * Globals::PI / 180));  // Tính quãng đường trượt theo x
            float yMove = slideDist * (sin(slideAngle * Globals::PI / 180));  // Tính quãng đường trượt theo y

            x += xMove;  // Cập nhật vị trí x
            y += yMove;  // Cập nhật vị trí y

            totalXMove += xMove;  // Cộng vào tổng quãng đường x
            totalYMove += yMove;  // Cộng vào tổng quãng đường y
            slideAmount -= slideDist;  // Giảm lượng trượt
        }
        else
        {
            slideAmount = 0;  // Đặt lại lượng trượt về 0 nếu hết
        }
    }

    updateCollisionBox();  // Cập nhật lại hộp va chạm sau khi di chuyển
    SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);  // Gộp hộp va chạm hiện tại và trước đó
}

// Hàm cập nhật hộp va chạm
void Entity::updateCollisionBox()
{
    collisionBox.x = x - collisionBox.w / 2;  // Đặt tọa độ x của hộp (giữa thực thể)
    collisionBox.y = y + collisionBoxYOffset;  // Đặt tọa độ y (điều chỉnh theo offset)
    collisionBox.w = collisionBoxW;  // Đặt chiều rộng mặc định
    collisionBox.h = collisionBoxH;  // Đặt chiều cao mặc định
}

// Hàm kiểm tra và xử lý va chạm
void Entity::updateCollisions()
{
    if (active && collideWithSolids && (moveSpeed > 0 || slideAmount > 0))  // Nếu thực thể hoạt động và có thể va chạm
    {
        list<Entity*> collisions;  // Danh sách các thực thể có thể va chạm

        // Tìm các thực thể có khả năng va chạm
        for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            if ((*entity)->active
                && (*entity)->type != this->type
                && (*entity)->solid
                && Entity::checkCollision(collisionBox, (*entity)->collisionBox))
            {
                collisions.push_back(*entity);  // Thêm vào danh sách va chạm
            }
        }

        // Nếu có va chạm tiềm năng
        if (collisions.size() > 0)
        {
            updateCollisionBox();  // Cập nhật lại hộp va chạm

            float boxTravelSize = 0;  // Kích thước bước kiểm tra va chạm
            if (collisionBox.w < collisionBox.h)
                boxTravelSize = collisionBox.w / 4;  // Dùng chiều nhỏ hơn để lấy kích thước bước
            else
                boxTravelSize = collisionBox.h / 4;

            SDL_Rect sampleBox = lastCollisionBox;  // Hộp mẫu để kiểm tra từng bước
            float movementAngle = Entity::angleBetweenTwoRects(lastCollisionBox, collisionBox);  // Góc di chuyển

            bool foundCollision = false;  // Cờ báo tìm thấy va chạm
            while (!foundCollision)
            {
                SDL_Rect intersection;  // Hộp giao nhau khi va chạm
                for (auto entity = collisions.begin(); entity != collisions.end(); entity++)
                {
                    if (SDL_IntersectRect(&sampleBox, &(*entity)->collisionBox, &intersection))
                    {
                        foundCollision = true;  // Đánh dấu đã tìm thấy va chạm
                        moveSpeed = 0;         // Dừng di chuyển
                        moving = false;        // Dừng trạng thái di chuyển
                        slideAngle = angleBetweenTwoEntities((*entity), this);  // Cập nhật góc trượt

                        // Xử lý giải quyết va chạm
                        if (intersection.w < intersection.h)  // Nếu giao nhau theo chiều ngang nhỏ hơn
                        {
                            if (lastCollisionBox.x + lastCollisionBox.w / 2 < (*entity)->collisionBox.x + (*entity)->collisionBox.w / 2)
                                sampleBox.x -= intersection.w;  // Di chuyển ra khỏi trái
                            else
                                sampleBox.x += intersection.w;  // Di chuyển ra khỏi phải
                        }
                        else  // Nếu giao nhau theo chiều dọc nhỏ hơn
                        {
                            if (lastCollisionBox.y + lastCollisionBox.h / 2 < (*entity)->collisionBox.y + (*entity)->collisionBox.h / 2)
                                sampleBox.y -= intersection.h;  // Di chuyển ra khỏi trên
                            else
                                sampleBox.y += intersection.h;  // Di chuyển ra khỏi dưới
                        }
                    }
                }

                // Thoát vòng lặp nếu tìm thấy va chạm hoặc hộp mẫu đã đến đích
                if (foundCollision || (sampleBox.x == collisionBox.x && sampleBox.y == collisionBox.y))
                    break;

                // Di chuyển hộp mẫu đến vị trí tiếp theo
                if (distanceBetweenTwoRects(sampleBox, collisionBox) > boxTravelSize)
                {
                    float xMove = boxTravelSize * (cos(movementAngle * Globals::PI / 180));
                    float yMove = boxTravelSize * (sin(movementAngle * Globals::PI / 180));

                    sampleBox.x += xMove;
                    sampleBox.y += yMove;
                }
                else
                {
                    sampleBox = collisionBox;  // Đặt hộp mẫu bằng hộp đích nếu gần đến
                }
            }

            if (foundCollision)
            {
                slideAmount = slideAmount / 2;  // Giảm lượng trượt
                x = sampleBox.x + sampleBox.w / 2;  // Cập nhật vị trí x
                y = sampleBox.y - collisionBoxYOffset;  // Cập nhật vị trí y
            }

            updateCollisionBox();  // Cập nhật lại hộp va chạm sau khi xử lý
        }
    }
}

// CÁC HÀM HỖ TRỢ

// Tính khoảng cách giữa hai hình chữ nhật
float Entity::distanceBetweenTwoRects(SDL_Rect& r1, SDL_Rect& r2)
{
    SDL_Point e1, e2;
    e1.x = r1.x + r1.w / 2;  // Tâm x của r1
    e1.y = r1.y + r1.h / 2;  // Tâm y của r1
    e2.x = r2.x + r2.w / 2;  // Tâm x của r2
    e2.y = r2.y + r2.h / 2;  // Tâm y của r2

    float d = abs(sqrt(pow(e2.x - e1.x, 2) + pow(e2.y - e1.y, 2)));  // Khoảng cách Euclidean
    return d;
}

// Tính khoảng cách giữa hai thực thể
float Entity::distanceBetweenTwoEntities(Entity* e1, Entity* e2)
{
    float d = abs(sqrt(pow(e2->x - e1->x, 2) + pow(e2->y - e1->y, 2)));  // Khoảng cách Euclidean
    return d;
}

// Tính góc giữa hai thực thể
float Entity::angleBetweenTwoEntities(Entity* e1, Entity* e2)
{
    float dx = e2->x - e1->x;  // Chênh lệch x
    float dy = e2->y - e1->y;  // Chênh lệch y

    return atan2(dy, dx) * 180 / Globals::PI;  // Tính góc theo độ
}

// Kiểm tra va chạm giữa hai hộp
bool Entity::checkCollision(SDL_Rect cbox1, SDL_Rect cbox2)
{
    SDL_Rect intersection;
    if (SDL_IntersectRect(&cbox1, &cbox2, &intersection))
    {
        return true;  // Có giao nhau
    }
    return false;  // Không giao nhau
}

// Chuyển đổi góc thành hướng
int Entity::angleToDirection(float angle)
{
    if ((angle >= 0 && angle <= 45) || (angle >= 315 && angle <= 360))
        return DIR_RIGHT;  // Hướng phải
    else if (angle >= 45 && angle <= 135)
        return DIR_DOWN;   // Hướng xuống
    else if (angle >= 135 && angle <= 225)
        return DIR_LEFT;   // Hướng trái
    else
        return DIR_UP;     // Hướng lên
}

// Tính góc giữa hai điểm
float Entity::angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2)
{
    float dx = cx2 - cx1;  // Chênh lệch x
    float dy = cy2 - cy1;  // Chênh lệch y

    return atan2(dy, dx) * 180 / Globals::PI;  // Tính góc theo độ
}

// Tính góc giữa hai hình chữ nhật
float Entity::angleBetweenTwoRects(SDL_Rect& r1, SDL_Rect& r2)
{
    float cx1 = r1.x + (r1.w / 2);  // Tâm x của r1
    float cy1 = r1.y + (r1.h / 2);  // Tâm y của r1
    float cx2 = r2.x + (r2.w / 2);  // Tâm x của r2
    float cy2 = r2.y + (r2.h / 2);  // Tâm y của r2

    return angleBetweenTwoPoints(cx1, cy1, cx2, cy2);  // Tính góc giữa hai tâm
}

// Khởi tạo danh sách thực thể toàn cục
list<Entity*> Entity::entities;

// So sánh hai thực thể để sắp xếp theo y
bool Entity::EntityCompare(const Entity* const& a, const Entity* const& b)
{
    if (a != 0 && b != 0)
    {
        return (a->y < b->y);  // Sắp xếp theo giá trị y tăng dần
    }
    else
    {
        return false;
    }
}

// Xóa các thực thể không hoạt động khỏi danh sách
void Entity::removeInactiveEntitiesFromList(list<Entity*>* entityList, bool deleteEntities)
{
    for (auto entity = entityList->begin(); entity != entityList->end();)
    {
        if (!(*entity)->active)  // Nếu thực thể không hoạt động
        {
            if (deleteEntities)
                delete (*entity);  // Xóa đối tượng nếu được yêu cầu
            entity = entityList->erase(entity);  // Xóa khỏi danh sách
        }
        else
        {
            entity++;  // Tiếp tục với thực thể tiếp theo
        }
    }
}

// Xóa tất cả thực thể khỏi danh sách
void Entity::removeAllFromList(list<Entity*>* entityList, bool deleteEntities)
{
    for (auto entity = entityList->begin(); entity != entityList->end();)
    {
        if (deleteEntities)
            delete (*entity);  // Xóa đối tượng nếu được yêu cầu
        entity = entityList->erase(entity);  // Xóa khỏi danh sách
    }
}