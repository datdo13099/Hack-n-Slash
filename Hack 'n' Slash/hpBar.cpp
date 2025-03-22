#include "hpBar.h"
#include <algorithm>

HPBar::HPBar(int initialX, int initialY, int width, int height, LivingEntity* target, SDL_Color barColor)
{
    x = initialX; // Tọa độ x ban đầu của thanh HP
    y = initialY; // Tọa độ y ban đầu của thanh HP
    barWidth = width; // Chiều rộng của thanh HP
    barHeight = height; // Chiều cao của thanh HP
    entity = target; // Con trỏ đến đối tượng LivingEntity mà thanh HP này theo dõi
    color = barColor; // Màu của thanh HP (do người dùng chỉ định)
    currentHp = target ? target->hp : 0; // Khởi tạo HP hiện tại: nếu target tồn tại thì lấy HP của target, không thì đặt bằng 0
}

void HPBar::draw()
{
    if (entity != NULL) // Kiểm tra xem đối tượng được theo dõi có tồn tại không
    {
        // Giảm mượt mà currentHp để khớp với HP thực tế của entity
        if (currentHp > entity->hp)
        {
            currentHp -= 0.5f; // Giảm dần currentHp (0.5f là tốc độ giảm, có thể điều chỉnh để mượt hơn)
            if (currentHp < entity->hp)
                currentHp = entity->hp; // Đảm bảo currentHp không nhỏ hơn HP thực tế của entity
        }

        // Vẽ viền ngoài của thanh HP
        SDL_SetRenderDrawColor(Globals::renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // Đặt màu trắng cho viền
        SDL_Rect barContainer = { x, y, barWidth, barHeight }; // Hình chữ nhật đại diện cho toàn bộ khung thanh HP
        SDL_RenderDrawRect(Globals::renderer, &barContainer); // Vẽ viền khung

        // Tính tỷ lệ phần trăm HP còn lại của entity, giới hạn trong khoảng [0, 1]
        float hpPercent = currentHp / (entity->hpMax * 1.0f); // Chia cho HP tối đa để ra phần trăm
        hpPercent = std::max(0.0f, std::min(1.0f, hpPercent)); // Đảm bảo giá trị nằm trong [0, 1]

        // Thay đổi màu thanh HP dựa trên mức HP còn lại
        if (hpPercent < 0.2f) // Nếu HP dưới 20%
        {
            SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); // Đặt màu đỏ để cảnh báo
        }
        else
        {
            SDL_SetRenderDrawColor(Globals::renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE); // Dùng màu gốc của thanh HP
        }

        // Vẽ phần HP còn lại bên trong khung
        SDL_Rect hpRect = { x + 2, y + 2, (barWidth - 4) * hpPercent, barHeight - 4 }; // Hình chữ nhật biểu thị HP còn lại, thu nhỏ 2 pixel mỗi bên để vừa viền
        SDL_RenderFillRect(Globals::renderer, &hpRect); // Tô màu cho phần HP còn lại
    }
}