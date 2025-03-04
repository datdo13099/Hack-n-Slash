#include <iostream>        // Thư viện chuẩn để sử dụng đầu vào/đầu ra
#include <fstream>         // Thư viện để làm việc với file
#include <string>          // Thư viện để làm việc với chuỗi ký tự
#include <sstream>         // Thư viện để xử lý luồng chuỗi (stringstream)
#include <list>            // Thư viện để sử dụng danh sách (list)
#include <SDL.h>           // Thư viện SDL để quản lý đồ họa và các tính năng game
#include "dataGroupType.h" // Bao gồm file header chứa định nghĩa kiểu dữ liệu nhóm (DataGroupType)

using namespace std;

#ifndef GROUP_H  // Kiểm tra nếu GROUP_H chưa được định nghĩa
#define GROUP_H  // Định nghĩa GROUP_H để tránh include file nhiều lần

// LỚP TRỪU TƯỢNG (ABSTRACT CLASS)
// Lớp Group quản lý dữ liệu của một frame
class Group
{
public:
    DataGroupType type;  // Mô tả tên, kiểu và các quy tắc khác của nhóm

    Group()  // Hàm khởi tạo mặc định
    {
        // Không làm gì...
    }

    Group(DataGroupType type)  // Hàm khởi tạo với tham số kiểu dữ liệu nhóm
    {
        this->type = type;  // Gán kiểu dữ liệu nhóm được truyền vào
    }

    // Trả về số lượng dữ liệu trong nhóm (hàm ảo thuần túy)
    virtual int numberOfDataInGroup() = 0;

    /*
     * Thêm dữ liệu vào nhóm bằng chuỗi. Chuyển đổi chuỗi thành dữ liệu đúng kiểu tùy theo loại nhóm
     * (hàm ảo thuần túy)
     */
    virtual void addToGroup(string str) = 0;

    // Vẽ dữ liệu của nhóm lên màn hình (hàm ảo thuần túy)
    virtual void draw() = 0;
};

#endif  // Kết thúc khối điều kiện định nghĩa GROUP_H