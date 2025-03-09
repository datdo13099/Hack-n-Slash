#include "globals.h"

// Định nghĩa hằng số PI
const float Globals::PI = 3.14159;  // Giá trị PI dùng cho các phép tính toán học

// Biến hữu ích cho nhà phát triển
bool Globals::debugging = false;  // Chế độ debug, mặc định là tắt (false)

// Các biến liên quan đến SDL
int Globals::ScreenWidth = 1280;   
int Globals::ScreenHeight = 704;   
int Globals::ScreenScale = 1;   
SDL_Renderer* Globals::renderer = NULL;  // Con trỏ tới renderer của SDL, khởi tạo là NULL

// Hàm cắt bỏ phần tiêu đề của chuỗi dữ liệu
string Globals::clipOffDataHeader(string data)
{
    int pos = data.find(":", 0);  // Tìm vị trí của dấu hai chấm trong chuỗi, trả về -1 nếu không tìm thấy
    if (pos != -1)  // Nếu tìm thấy dấu hai chấm
    {
        data = data.substr(pos + 1, data.length() - pos + 2);  // Cắt chuỗi từ sau dấu hai chấm đến hết
    }
    return data;  // Trả về chuỗi đã được xử lý
}