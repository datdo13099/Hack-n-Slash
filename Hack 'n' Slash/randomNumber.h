#ifndef __RANDOM_H__  // Kiểm tra nếu __RANDOM_H__ chưa được định nghĩa
#define __RANDOM_H__  // Định nghĩa __RANDOM_H__ để tránh include file nhiều lần

#include <ctime>     // Thư viện để lấy thời gian hiện tại (dùng để khởi tạo seed cho số ngẫu nhiên)
#include <cstdlib>   // Thư viện chứa hàm rand() và srand() để tạo số ngẫu nhiên

int getRandomNumber(int mod);  // Hàm trả về một số ngẫu nhiên trong khoảng từ 0 đến (mod - 1)

#endif  // Kết thúc khối điều kiện định nghĩa __RANDOM_H__