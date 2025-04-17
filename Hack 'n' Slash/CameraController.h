#ifndef CAMERACONTROLLER
#define CAMERACONTROLLER

#include "globals.h"
#include "entity.h"

// Lớp CameraController: Điều khiển camera trong trò chơi
class CameraController 
{
public:
    Entity* target = NULL; // Đối tượng mà camera sẽ theo dõi (mặc định là NULL)
    float lerp = 2.6f; // Hệ số nội suy (lerp) để di chuyển camera mượt mà

    void update(); // Hàm cập nhật vị trí của camera
};

#endif