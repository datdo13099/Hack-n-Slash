#ifndef TIMECONTROLLER  // Kiểm tra nếu TIMECONTROLLER chưa được định nghĩa
#define TIMECONTROLLER  // Định nghĩa TIMECONTROLLER để tránh include file nhiều lần

#include <iostream>  // Thư viện chuẩn để sử dụng đầu vào/đầu ra (có thể dùng để debug)
#include "SDL.h"     // Thư viện SDL để quản lý thời gian và các tính năng game

class TimeController  // Định nghĩa lớp TimeController để quản lý thời gian trong game
{
public:
    // Giá trị tham chiếu (hằng số tĩnh) để biểu thị trạng thái chơi và tạm dừng
    static const int PLAY_STATE, PAUSE_STATE;

    int timeState;  // Trạng thái thời gian hiện tại (ví dụ: đang chạy hay tạm dừng)
    Uint32 lastUpdate;  // Thời điểm cuối cùng mà số tick (xung nhịp) được kiểm tra, kiểu Uint32 từ SDL
    float dT;  // Delta time (thời gian chênh lệch), tính bằng giây (1 = 1 giây, 0.5 = nửa giây). Là thời gian kể từ frame cuối cùng được vẽ lên màn hình

    TimeController();  // Hàm khởi tạo của TimeController
    void updateTime();  // Hàm cập nhật thời gian: cập nhật lastUpdate và tính toán dT
    void pause();  // Hàm tạm dừng thời gian
    void resume();  // Hàm tiếp tục thời gian sau khi tạm dừng
    void reset();  // Hàm đặt lại thời gian về trạng thái ban đầu

    static TimeController timeController;  // Biến tĩnh (static) để tạo một thực thể duy nhất của TimeController (singleton pattern)
};

#endif  // Kết thúc khối điều kiện định nghĩa TIMECONTROLLER