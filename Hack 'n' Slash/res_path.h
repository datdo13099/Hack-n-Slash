#ifndef RES_PATH_H  // Kiểm tra nếu RES_PATH_H chưa được định nghĩa
#define RES_PATH_H  // Định nghĩa RES_PATH_H để tránh include file nhiều lần

#include <iostream>  // Thư viện chuẩn để sử dụng đầu vào/đầu ra (có thể dùng để debug)
#include <string>    // Thư viện để làm việc với chuỗi ký tự (std::string)
#include <SDL.h>     // Thư viện SDL để quản lý tài nguyên game và các tính năng liên quan

/*
 * Hàm lấy đường dẫn tài nguyên cho các tài nguyên nằm trong thư mục con res/subDir
 * Giả định cấu trúc thư mục của dự án như sau:
 * bin/
 *   tệp thực thi (executable)
 * res/
 *   Lesson1/
 *   Lesson2/
 *
 * Đường dẫn trả về sẽ có dạng Lessons/res/subDir
 */
std::string getResourcePath(const std::string& subDir = "");  // Hàm trả về đường dẫn tài nguyên, tham số subDir là tùy chọn (mặc định là chuỗi rỗng)

#endif  // Kết thúc khối điều kiện định nghĩa RES_PATH_H