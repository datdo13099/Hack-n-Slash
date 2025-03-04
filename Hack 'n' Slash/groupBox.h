#ifndef GROUPBOX_H  // Kiểm tra nếu GROUPBOX_H chưa được định nghĩa
#define GROUPBOX_H  // Định nghĩa GROUPBOX_H để tránh include file nhiều lần

#include "group.h"  // Bao gồm file header của lớp Group, lớp cha của GroupBox

class GroupBox : public Group  // Định nghĩa lớp GroupBox, kế thừa công khai từ Group
{
public:
    list<SDL_Rect> data;  // Danh sách các hình chữ nhật SDL (SDL_Rect) được lưu trữ trong nhóm

    GroupBox(DataGroupType type)  // Hàm khởi tạo của GroupBox, nhận tham số kiểu dữ liệu nhóm
    {
        this->type = type;  // Gán kiểu dữ liệu nhóm được truyền vào
    }

    int numberOfDataInGroup()  // Hàm trả về số lượng dữ liệu trong nhóm
    {
        return data.size();  // Trả về kích thước của danh sách data
    }

    void addToGroup(string str)  // Hàm thêm một hình chữ nhật từ chuỗi ký tự vào nhóm
    {
        // Hy vọng cách này hoạt động :/
        stringstream ss;  // Tạo một luồng chuỗi để phân tích cú pháp
        ss << str;        // Đưa chuỗi vào luồng
        SDL_Rect box;     // Tạo một hình chữ nhật SDL để lưu trữ tọa độ x, y, chiều rộng (w) và chiều cao (h)
        ss >> box.x >> box.y >> box.w >> box.h;  // Đọc lần lượt x, y, w, h từ chuỗi (giả định định dạng "x y w h")

        data.push_back(box);  // Thêm hình chữ nhật vừa tạo vào danh sách
    }

    void draw()  // Hàm vẽ nhóm hình chữ nhật lên màn hình
    {
        // TODO: Chưa được triển khai (cần hoàn thiện sau)
    }
};

#endif  // Kết thúc khối điều kiện định nghĩa GROUPBOX_H