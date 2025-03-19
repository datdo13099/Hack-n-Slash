#ifndef GLOBALS  // Kiểm tra nếu GLOBALS chưa được định nghĩa
#define GLOBALS  // Định nghĩa GLOBALS để tránh include file nhiều lần

#include <string>          // Thư viện để làm việc với chuỗi ký tự
#include <iostream>        // Thư viện chuẩn để sử dụng đầu vào/đầu ra
#include <SDL.h>           // Thư viện SDL để quản lý đồ họa và các tính năng game
#include "randomNumber.h"  // Bao gồm file header chứa hàm tạo số ngẫu nhiên

using namespace std;

class Globals  // Định nghĩa lớp Globals để chứa các biến và hàm toàn cục
{
public:
	// Trợ giúp toán học
	static const float PI;  // Hằng số PI (3.14159...) dùng trong tính toán

	// Hữu ích cho nhà phát triển
	static bool debugging;  // Biến tĩnh để bật/tắt chế độ debug

	// Liên quan đến SDL
	static int ScreenWidth, ScreenHeight, ScreenScale;  // Chiều rộng, chiều cao và tỷ lệ màn hình
	static SDL_Renderer* renderer;  // Con trỏ tới renderer của SDL để vẽ đồ họa

	// Loại bỏ phần tiêu đề của dữ liệu. Ví dụ: "clip: 50 114 44 49" sẽ thành "50 114 44 49"
	static string clipOffDataHeader(string data);  // Hàm tĩnh cắt bỏ tiêu đề khỏi chuỗi dữ liệu

	//camera
	static SDL_Rect camera;
};

#endif  // Kết thúc khối điều kiện định nghĩa GLOBALS