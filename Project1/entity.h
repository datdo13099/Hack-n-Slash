#ifndef ENTITY
#define ENTITY

#include "globals.h"
#include "timeController.h"
#include "animationSet.h"

class Entity
{
public:
    // hằng số chỉ hướng
    static const int DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE;

    // thuộc tính entity
    int state;

    float x, y;
    int direction;
    bool solid = true;  // có phải vật thể cứng không
    bool collideWithSolids = true;  // có va chạm với vật thể cứng không
    bool active = true;  // entity có đang hoạt động không
    string type = "entity";  // loại entity (vd: hero, enemy, wall)
    bool moving;  // entity có đang di chuyển không
    float angle;  // góc di chuyển (độ)
    float moveSpeed;
    float moveSpeedMax;
    float slideAngle;  // góc trượt
    float slideAmount;  // lực trượt
    float moveLerp = 4;
    float totalXMove, totalYMove; // theo dõi tổng chuyển động x, y trong mỗi lượt di chuyển, phòng trường hợp cần hoàn tác

    SDL_Rect collisionBox; // hình chữ nhật xác định kích thước của entity, dùng để phát hiện va chạm
    SDL_Rect lastCollisionBox; // vị trí của hộp va chạm trong lần cập nhật trước
    int collisionBoxW, collisionBoxH; // kích thước mặc định của hộp va chạm
    float collisionBoxYOffset; // độ lệch theo trục y, xác định vị trí đặt hộp va chạm dựa trên giá trị y của entity

    AnimationSet* animSet; // tập hợp tất cả các animation mà entity có thể sử dụng
    Animation* currentAnim; // animation hiện tại của entity
    Frame* currentFrame; // frame hiện tại trong animation mà entity đang dùng
    float frameTimer; // bộ đếm thời gian giúp chuyển frame mượt mà

    // CÁC HÀM ẢO
    virtual void update(); // cập nhật trạng thái của entity mỗi frame
    virtual void draw(); // vẽ entity lên màn hình

    virtual void move(float angle); // di chuyển entity theo góc chỉ định
    virtual void updateMovement(); // cập nhật thông tin di chuyển của entity
    virtual void updateCollisionBox(); // cập nhật vị trí hộp va chạm theo chuyển động

    virtual void changeAnimation(int newState, bool resetFrameToBeginning) = 0; // thay đổi animation theo trạng thái mới, có thể reset frame về ban đầu
    virtual void updateCollisions(); // cách xử lý va chạm trong thế giới game

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

};

#endif