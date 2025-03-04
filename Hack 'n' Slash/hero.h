#ifndef HERO  // Kiểm tra nếu HERO chưa được định nghĩa
#define HERO  // Định nghĩa HERO để tránh include file nhiều lần

#include "globals.h"       // Bao gồm file header chứa các định nghĩa toàn cục
#include "livingEntity.h"  // Bao gồm file header của lớp LivingEntity, lớp cha của Hero

class Hero : public LivingEntity  // Định nghĩa lớp Hero, kế thừa công khai từ LivingEntity
{
public:
    // Các biến tham chiếu (hằng số tĩnh) để định nghĩa tên animation của Hero
    static const string HERO_ANIM_UP;           // Animation khi di chuyển lên
    static const string HERO_ANIM_DOWN;         // Animation khi di chuyển xuống
    static const string HERO_ANIM_LEFT;         // Animation khi di chuyển sang trái
    static const string HERO_ANIM_RIGHT;        // Animation khi di chuyển sang phải
    static const string HERO_ANIM_IDLE_UP;      // Animation khi đứng yên hướng lên
    static const string HERO_ANIM_IDLE_DOWN;    // Animation khi đứng yên hướng xuống
    static const string HERO_ANIM_IDLE_LEFT;    // Animation khi đứng yên hướng trái
    static const string HERO_ANIM_IDLE_RIGHT;   // Animation khi đứng yên hướng phải
    static const string HERO_SLASH_ANIM_UP;     // Animation khi tấn công hướng lên
    static const string HERO_SLASH_ANIM_DOWN;   // Animation khi tấn công hướng xuống
    static const string HERO_SLASH_ANIM_LEFT;   // Animation khi tấn công hướng trái
    static const string HERO_SLASH_ANIM_RIGHT;  // Animation khi tấn công hướng phải
    static const string HERO_DASH_ANIM_UP;      // Animation khi lướt nhanh hướng lên
    static const string HERO_DASH_ANIM_DOWN;    // Animation khi lướt nhanh hướng xuống
    static const string HERO_DASH_ANIM_LEFT;    // Animation khi lướt nhanh hướng trái
    static const string HERO_DASH_ANIM_RIGHT;   // Animation khi lướt nhanh hướng phải
    static const string HERO_ANIM_DIE;          // Animation khi Hero chết

    // Các trạng thái của Hero (hằng số tĩnh)
    static const int HERO_STATE_IDLE;  // Trạng thái đứng yên
    static const int HERO_STATE_MOVE;  // Trạng thái di chuyển
    static const int HERO_STATE_SLASH; // Trạng thái tấn công
    static const int HERO_STATE_DASH;  // Trạng thái lướt nhanh
    static const int HERO_STATE_DEAD;  // Trạng thái đã chết

    Hero(AnimationSet* animSet);  // Hàm khởi tạo của Hero, nhận con trỏ tới AnimationSet
    void update();  // Hàm cập nhật trạng thái của Hero mỗi frame
    void slash();   // Hàm thực hiện hành động tấn công
    void dash();    // Hàm thực hiện hành động lướt nhanh
    void die();     // Hàm xử lý khi Hero chết
    void revive();  // Hàm hồi sinh Hero
    void changeAnimation(int newState, bool resetFrameToBeginning);  // Hàm thay đổi animation theo trạng thái mới
    void updateAnimation();  // Hàm cập nhật animation hiện tại
    void updateDamages();    // Hàm cập nhật sát thương mà Hero nhận vào
};

#endif  // Kết thúc khối điều kiện định nghĩa HERO