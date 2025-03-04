#ifndef GROUPSTRING_H  // Kiểm tra nếu GROUPSTRING_H chưa được định nghĩa
#define GROUPSTRING_H  // Định nghĩa GROUPSTRING_H để tránh include file nhiều lần

#include "group.h"  // Bao gồm file header của lớp Group, lớp cha của GroupString

class GroupString : public Group  // Định nghĩa lớp GroupString, kế thừa công khai từ Group
{
public:
    list<string> data;  // Danh sách các chuỗi ký tự được lưu trữ trong nhóm

    GroupString(DataGroupType type)  // Hàm khởi tạo của GroupString, nhận tham số kiểu dữ liệu nhóm
    {
        this->type = type;  // Gán kiểu dữ liệu nhóm được truyền vào
    }

    int numberOfDataInGroup()  // Hàm trả về số lượng dữ liệu trong nhóm
    {
        return data.size();  // Trả về kích thước của danh sách data
    }

    void addToGroup(string str)  // Hàm thêm một chuỗi vào nhóm
    {
        // Nếu chuỗi bắt đầu bằng khoảng trắng, xóa khoảng trắng đó đi
        if (str[0] == ' ')
        {
            str.erase(0, 1);  // Xóa ký tự đầu tiên (khoảng trắng)
        }

        data.push_back(str);  // Thêm chuỗi đã xử lý vào danh sách
    }

    void draw()  // Hàm vẽ nhóm chuỗi lên màn hình
    {
        // TODO: Chưa được triển khai (cần hoàn thiện sau)
    }
};

#endif  // Kết thúc khối điều kiện định nghĩa GROUPSTRING_H