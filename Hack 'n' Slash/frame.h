#ifndef FRAME  // Kiểm tra nếu FRAME chưa được định nghĩa
#define FRAME  // Định nghĩa FRAME để tránh include file nhiều lần

#include <iostream>           // Thư viện chuẩn để sử dụng đầu vào/đầu ra
#include <list>               // Thư viện để sử dụng danh sách (list)
#include <fstream>            // Thư viện để làm việc với file
#include <sstream>            // Thư viện để xử lý luồng chuỗi (stringstream)
#include "SDL.h"              // Thư viện SDL để quản lý đồ họa và các tính năng game
#include "drawing_functions.h" // Bao gồm file header chứa các hàm hỗ trợ vẽ
#include "globals.h"           // Bao gồm file header chứa các định nghĩa toàn cục
#include "groupBuilder.h"      // Bao gồm file header của lớp GroupBuilder

using namespace std;

class Frame  // Định nghĩa lớp Frame để quản lý một khung hình trong animation
{
public:
    int frameNumber;  // Số thứ tự hoặc chỉ số của khung hình
    SDL_Rect clip;    // Vùng trên spritesheet nơi khung hình này nằm
    float duration;   // Thời gian khung hình được hiển thị (tính bằng giây)
    SDL_Point offSet; // Điểm pivot để căn chỉnh khung hình trong animation

    list<Group*> frameData;  // Danh sách các con trỏ tới Group, chứa dữ liệu bổ sung cho khung hình

    void Draw(SDL_Texture* spriteSheet, float x, float y);  // Hàm vẽ khung hình lên màn hình tại tọa độ (x, y)

    void loadFrame(ifstream& file, list<DataGroupType>& groupTypes);  // Hàm tải thông tin khung hình từ file
};

#endif  // Kết thúc khối điều kiện định nghĩa FRAME