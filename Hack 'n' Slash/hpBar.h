#ifndef HPBAR // Nếu HPBAR chưa được định nghĩa (kiểm tra để tránh trùng lặp khi include)
#define HPBAR // Định nghĩa HPBAR để đánh dấu đã include file này

#include "livingEntity.h" // Include file định nghĩa lớp LivingEntity
#include <SDL.h> // Include thư viện SDL để sử dụng SDL_Color, SDL_Rect, v.v.

// Lớp HPBar: Biểu diễn thanh máu (HP bar) của một thực thể sống
class HPBar
{
public:
    int x, y; // Tọa độ x, y của thanh HP trên màn hình
    int barWidth = 200, barHeight = 30; // Chiều rộng và chiều cao mặc định của thanh HP
    LivingEntity* entity = NULL; // Con trỏ tới đối tượng LivingEntity mà thanh HP này theo dõi, mặc định là NULL
    SDL_Color color; // Màu của thanh HP, sử dụng cấu trúc SDL_Color (r, g, b, a)
    float currentHp; // Giá trị HP hiện tại được hiển thị trên thanh (có thể khác với HP thực tế của entity để tạo hiệu ứng mượt)

    // Constructor: Khởi tạo thanh HP với các tham số tùy chọn
    HPBar(int initialX = 0, int initialY = 0, int width = 200, int height = 30, LivingEntity* target = NULL, SDL_Color barColor = { 255, 255, 255, 255 });
    // - initialX, initialY: Tọa độ ban đầu (mặc định là 0, 0)
    // - width, height: Kích thước thanh HP (mặc định 200x30)
    // - target: Đối tượng LivingEntity cần theo dõi (mặc định là NULL)
    // - barColor: Màu của thanh HP (mặc định là trắng với alpha 255)

    void draw(); // Hàm vẽ thanh HP lên màn hình
};

#endif // Kết thúc khối điều kiện #ifndef, đảm bảo chỉ định nghĩa lớp HPBar một lần
