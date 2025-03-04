#ifndef WALL  // Kiểm tra nếu WALL chưa được định nghĩa
#define WALL  // Định nghĩa WALL để tránh việc include file nhiều lần

#include "entity.h"  // Bao gồm file header của lớp Entity, lớp cha của Wall

class Wall : public Entity  // Định nghĩa lớp Wall, kế thừa công khai từ Entity
{
public:
    Wall(AnimationSet* animSet);  // Hàm khởi tạo của Wall, nhận vào một con trỏ đến AnimationSet
    void update();  // Hàm cập nhật trạng thái của Wall (có thể xử lý logic mỗi frame)
    void changeAnimation(int newState, bool resetFrameToBeginning);  // Hàm thay đổi animation, với tham số trạng thái mới và cờ đặt lại frame về đầu

    void updateCollisions()  // Hàm cập nhật va chạm
    {
        // Không làm gì cả. Trong game này, tường không di chuyển hay bị đẩy
    }
};

#endif  // Kết thúc khối điều kiện định nghĩa WALL