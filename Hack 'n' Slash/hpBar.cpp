#include "hpBar.h"
#include <algorithm>

// Constructor: Khởi tạo thanh HP với các tham số ban đầu
HPBar::HPBar(int initialX, int initialY, int width, int height, LivingEntity* target, SDL_Color barColor)
{
    x = initialX; // Tọa độ x ban đầu
    y = initialY; // Tọa độ y ban đầu
    barWidth = width; // Chiều rộng của thanh HP
    barHeight = height; // Chiều cao của thanh HP
    entity = target; // Đối tượng LivingEntity mà thanh HP theo dõi
    color = barColor; // Màu của thanh HP
    currentHp = target ? target->hp : 0; // HP hiện tại, nếu không có đối tượng thì mặc định là 0
}

// Hàm vẽ thanh HP lên màn hình
void HPBar::draw()
{
    if (entity != NULL) // Kiểm tra nếu đối tượng LivingEntity tồn tại
    {
        // Giảm dần currentHp để khớp với HP thực tế của đối tượng (tạo hiệu ứng mượt)
        if (currentHp > entity->hp)
        {
            currentHp -= 0.5f; // Giảm giá trị currentHp
            if (currentHp < entity->hp) // Đảm bảo không giảm quá HP thực tế
                currentHp = entity->hp;
        }

        // Vẽ viền ngoài của thanh HP
        SDL_SetRenderDrawColor(Globals::renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // Màu trắng
        SDL_Rect barContainer = { x, y, barWidth, barHeight }; // Hộp chữ nhật viền ngoài
        SDL_RenderDrawRect(Globals::renderer, &barContainer); // Vẽ viền

        // Tính phần trăm HP còn lại
        float hpPercent = currentHp / (entity->hpMax * 1.0f); // Tỷ lệ HP hiện tại so với HP tối đa
        hpPercent = std::max(0.0f, std::min(1.0f, hpPercent)); // Đảm bảo giá trị nằm trong khoảng [0, 1]

        // Thay đổi màu thanh HP dựa trên phần trăm HP còn lại
        if (hpPercent < 0.2f) // Nếu HP còn dưới 20%
        {
            SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); // Màu đỏ
        }
        else // Nếu HP còn trên 20%
        {
            SDL_SetRenderDrawColor(Globals::renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE); // Màu mặc định
        }

        // Vẽ phần thanh HP đã được lấp đầy
        SDL_Rect hpRect = { x + 2, y + 2, static_cast<int>((barWidth - 4) * hpPercent), barHeight - 4 }; // Hộp chữ nhật bên trong
        SDL_RenderFillRect(Globals::renderer, &hpRect); // Vẽ phần lấp đầy
    }
}
