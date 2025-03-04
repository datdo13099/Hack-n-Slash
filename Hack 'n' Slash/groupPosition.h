#ifndef GROUPPOS_H  // Kiểm tra nếu GROUPPOS_H chưa được định nghĩa
#define GROUPPOS_H  // Định nghĩa GROUPPOS_H để tránh include file nhiều lần

#include "group.h"  // Bao gồm file header của lớp Group, lớp cha của GroupPosition

class GroupPosition : public Group  // Định nghĩa lớp GroupPosition, kế thừa công khai từ Group
{
public:
    list<SDL_Point> data;  // Danh sách các điểm SDL (SDL_Point) được lưu trữ trong nhóm

    GroupPosition(DataGroupType type)  // Hàm khởi tạo của GroupPosition, nhận tham số kiểu dữ liệu nhóm
    {
        this->type = type;  // Gán kiểu dữ liệu nhóm được truyền vào
    }

    int numberOfDataInGroup()  // Hàm trả về số lượng dữ liệu trong nhóm
    {
        return data.size();  // Trả về kích thước của danh sách data
    }

    void addToGroup(string str)  // Hàm thêm một điểm từ chuỗi ký tự vào nhóm
    {
        // Hy vọng cách này hoạt động :/
        stringstream ss;  // Tạo một luồng chuỗi để phân tích cú pháp
        ss << str;        // Đưa chuỗi vào luồng
        SDL_Point pt;     // Tạo một điểm SDL để lưu trữ tọa độ x, y
        ss >> pt.x >> pt.y;  // Đọc tọa độ x và y từ chuỗi (giả định định dạng "x y")

        data.push_back(pt);  // Thêm điểm vừa tạo vào danh sách
    }

    void draw()  // Hàm vẽ nhóm vị trí lên màn hình
    {
        // TODO: Chưa được triển khai (cần hoàn thiện sau)
    }
};

#endif  // Kết thúc khối điều kiện định nghĩa GROUPPOS_H