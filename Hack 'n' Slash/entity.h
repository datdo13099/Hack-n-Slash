#ifndef ENTITY  // Kiểm tra nếu ENTITY chưa được định nghĩa
#define ENTITY  // Định nghĩa ENTITY để tránh include file nhiều lần

#include <limits>
#include <algorithm>
#include "globals.h"
#include "timeController.h"
#include "animationSet.h"

// Lớp trừu tượng (Abstract Class). Không thể khởi tạo trực tiếp đối tượng kiểu Entity, ví dụ: Entity e;
class Entity
{
public:
    // Hằng số tham chiếu cho hướng di chuyển
    static const int DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE;

    // Nhãn nhanh để xem thực thể đang làm gì
    int state;

    float x, y;               // Tọa độ vị trí của thực thể
    int direction;            // Hướng hiện tại của thực thể
    bool solid = true;        // Thực thể này có rắn không, các vật khác có thể đi xuyên qua không
    bool collideWithSolids = true;  // Thực thể có va chạm với các vật rắn khác không
    bool dieOnSolids = false; //hữu ích nếu entity cần chết khi chạm vật rắn (ví dụ: đạn chạm tường)
    bool active = true;       // Thực thể đang bật hay tắt
    string type = "entity";   // Loại thực thể (ví dụ: hero, enemy, wall, v.v.)
    bool moving;              // Thực thể có đang di chuyển không
    float angle;              // Góc di chuyển của thực thể (theo độ, 360 độ)
    float moveSpeed;          // Tốc độ di chuyển hiện tại
    float moveSpeedMax;       // Tốc độ di chuyển tối đa
    float slideAngle;         // Góc bị đẩy khi trượt
    float slideAmount;        // Lượng đẩy trong hướng slideAngle
    float moveLerp = 4;       // Hệ số nội suy (lerp) cho chuyển động mượt mà
    float totalXMove, totalYMove;  // Theo dõi tổng quãng đường di chuyển theo x, y trong mỗi lượt di chuyển

    SDL_Rect collisionBox;     // Hộp va chạm mô tả kích thước thực thể, dùng để phát hiện va chạm
    SDL_Rect lastCollisionBox; // Vị trí hộp va chạm ở frame trước
    int collisionBoxW, collisionBoxH;  // Kích thước mặc định của hộp va chạm
    float collisionBoxYOffset;  // Độ lệch của hộp va chạm so với tọa độ y của thực thể

    AnimationSet* animSet;     // Tập hợp tất cả animation mà thực thể có thể sử dụng
    Animation* currentAnim;    // Animation hiện tại mà thực thể đang dùng
    Frame* currentFrame;       // Khung hình hiện tại trong animation đang dùng
    float frameTimer;          // Bộ đếm thời gian hỗ trợ chuyển đổi giữa các khung hình

    // CÁC HÀM ẢO (VIRTUAL FUNCTIONS)
    virtual void update();     // Hàm cập nhật trạng thái thực thể mỗi frame
    virtual void draw();       // Hàm vẽ thực thể lên màn hình

    virtual void move(float angle);  // Hàm di chuyển thực thể theo góc nhất định
    virtual void updateMovement();   // Hàm cập nhật chuyển động của thực thể
    virtual void updateCollisionBox();  // Hàm cập nhật hộp va chạm của thực thể

    virtual void changeAnimation(int newState, bool resetFrameToBeginning) = 0;  // Hàm ảo thuần túy: thay đổi animation
    virtual void updateCollisions();  // Hàm cập nhật va chạm với các vật thể trong thế giới
    virtual void crashOntoSolid() { ; } //nếu dieOnSolids là true, hàm này xử lý việc chết

    //HÀM HỖ TRỢ
    static float SweptAABB(SDL_Rect movingBox, float vx, float vy, SDL_Rect otherBox, float& normalX, float& normalY);
    static float distanceBetweenTwoRects(SDL_Rect& r1, SDL_Rect& r2);
    static float distanceBetweenTwoEntities(Entity* e1, Entity* e2);
    static float distanceBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
    static float angleBetweenTwoEntities(Entity* e1, Entity* e2);
    static bool checkCollision(SDL_Rect cbox1, SDL_Rect cbox2);
    static int angleToDirection(float angle);
    static float angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
    static float angleBetweenTwoRects(SDL_Rect& r1, SDL_Rect& r2);

    // Danh sách toàn cục các thực thể để tham chiếu bất cứ lúc nào
    static list<Entity*> entities;

    static bool EntityCompare(const Entity* const& a, const Entity* const& b);  // So sánh hai thực thể để sắp xếp (dựa trên giá trị y)
    static void removeInactiveEntitiesFromList(list<Entity*>* entityList, bool deleteEntities);  // Xóa các thực thể không hoạt động khỏi danh sách
    static void removeAllFromList(list<Entity*>* entityList, bool deleteEntities);  // Xóa tất cả thực thể khỏi danh sách
};

#endif