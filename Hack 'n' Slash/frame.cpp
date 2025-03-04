#include "frame.h"

// Hàm vẽ khung hình lên màn hình
void Frame::Draw(SDL_Texture* spriteSheet, float x, float y)
{
    SDL_Rect dest;  // Hình chữ nhật đích xác định vị trí vẽ khung hình
    dest.x = x - offSet.x;  // Tọa độ x, điều chỉnh theo điểm pivot (offSet)
    dest.y = y - offSet.y;  // Tọa độ y, điều chỉnh theo điểm pivot (offSet)
    dest.w = clip.w;        // Chiều rộng lấy từ clip
    dest.h = clip.h;        // Chiều cao lấy từ clip

    // Gọi hàm renderTexture để vẽ khung hình từ spriteSheet lên renderer
    renderTexture(spriteSheet, Globals::renderer, dest, &clip);
}

// Hàm tải thông tin khung hình từ file
void Frame::loadFrame(ifstream& file, list<DataGroupType>& groupTypes)
{
    // Tạo các nhóm dữ liệu cho khung hình dựa trên danh sách groupTypes
    GroupBuilder::buildGroups(groupTypes, frameData);

    string buffer;  // Biến tạm để lưu dòng đọc từ file

    // Đọc dòng "frame" - dữ liệu giả chỉ để đánh dấu đây là khung hình
    getline(file, buffer);

    // Đọc và xử lý thông tin clip (vùng trên spritesheet)
    getline(file, buffer);
    stringstream ss;  // Dùng stringstream để phân tích chuỗi
    buffer = Globals::clipOffDataHeader(buffer);  // Loại bỏ tiêu đề (ví dụ: "clip: ")
    ss << buffer;  // Đưa chuỗi vào luồng
    ss >> clip.x >> clip.y >> clip.w >> clip.h;  // Đọc tọa độ x, y, chiều rộng, chiều cao

    // Đọc và xử lý thông tin offset (điểm pivot)
    getline(file, buffer);
    ss.clear();  // Xóa luồng để dùng lại
    buffer = Globals::clipOffDataHeader(buffer);  // Loại bỏ tiêu đề (ví dụ: "offset: ")
    ss << buffer;
    ss >> offSet.x >> offSet.y;  // Đọc tọa độ x, y của offset

    // Đọc và xử lý thông tin duration (thời gian hiển thị)
    getline(file, buffer);
    ss.clear();  // Xóa luồng
    buffer = Globals::clipOffDataHeader(buffer);  // Loại bỏ tiêu đề (ví dụ: "duration: ")
    ss << buffer;
    ss >> duration;  // Đọc thời gian

    // Đọc và xử lý thông tin frameNumber (chỉ số khung hình)
    getline(file, buffer);
    ss.clear();  // Xóa luồng
    buffer = Globals::clipOffDataHeader(buffer);  // Loại bỏ tiêu đề (ví dụ: "index: ")
    ss << buffer;
    ss >> frameNumber;  // Đọc chỉ số khung hình

    // Tải các nhóm dữ liệu bổ sung (như hitBox, collisionBox) từ file
    GroupBuilder::loadGroups(file, frameData);
}