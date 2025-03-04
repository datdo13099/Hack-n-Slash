#ifndef GROUPNUMBER_H  // Kiểm tra nếu GROUPNUMBER_H chưa được định nghĩa
#define GROUPNUMBER_H  // Định nghĩa GROUPNUMBER_H để tránh include file nhiều lần

#include "group.h"  // Bao gồm file header của lớp Group, lớp cha của GroupNumber

class GroupNumber : public Group  // Định nghĩa lớp GroupNumber, kế thừa công khai từ Group
{
public:
    list<float> data;  // Danh sách các số thực (float) được lưu trữ trong nhóm

    GroupNumber(DataGroupType type)  // Hàm khởi tạo của GroupNumber, nhận tham số kiểu dữ liệu nhóm
    {
        this->type = type;  // Gán kiểu dữ liệu nhóm được truyền vào
    }

    int numberOfDataInGroup()  // Hàm trả về số lượng dữ liệu trong nhóm
    {
        return data.size();  // Trả về kích thước của danh sách data
    }

    void addToGroup(string str)  // Hàm thêm một số thực từ chuỗi ký tự vào nhóm
    {
        // Hy vọng cách này hoạt động :/
        stringstream ss;  // Tạo một luồng chuỗi để phân tích cú pháp
        ss << str;        // Đưa chuỗi vào luồng
        float r;          // Biến để lưu số thực được trích xuất
        ss >> r;          // Đọc số thực từ chuỗi

        data.push_back(r);  // Thêm số thực vừa trích xuất vào danh sách
    }

    void draw()  // Hàm vẽ nhóm số lên màn hình
    {
        // TODO: Chưa được triển khai (cần hoàn thiện sau)
    }
};

#endif  // Kết thúc khối điều kiện định nghĩa GROUPNUMBER_H