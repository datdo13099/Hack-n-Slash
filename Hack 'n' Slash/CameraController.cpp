#include "CameraController.h"

// Hàm cập nhật vị trí của camera
void CameraController::update() 
{
    if (target != NULL) { // Kiểm tra nếu có đối tượng để theo dõi
        // Lấy tọa độ mục tiêu (giữa màn hình, dựa trên vị trí của đối tượng)
        float targetX = target->x - Globals::camera.w / 2;
        float targetY = target->y - Globals::camera.h / 2;

        // Di chuyển camera dần dần đến vị trí mục tiêu (sử dụng nội suy tuyến tính - lerp)
        Globals::camera.x += ((targetX)-Globals::camera.x) * lerp * TimeController::timeController.dT;
        Globals::camera.y += ((targetY)-Globals::camera.y) * lerp * TimeController::timeController.dT;

        // Nếu muốn camera khóa cứng vào đối tượng (không di chuyển mượt):
        /* Globals::camera.x = targetX;
         Globals::camera.y = targetY;*/
    }
    // Nếu không có đối tượng để theo dõi, camera sẽ không di chuyển
}