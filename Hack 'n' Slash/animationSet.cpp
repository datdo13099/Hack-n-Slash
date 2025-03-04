#include "animationSet.h"

// Hàm hủy AnimationSet để giải phóng tài nguyên
AnimationSet::~AnimationSet()
{
    cleanup(spriteSheet);  // Giải phóng spriteSheet
    if (whiteSpriteSheet != NULL)
    {
        cleanup(whiteSpriteSheet);  // Giải phóng whiteSpriteSheet nếu có
    }
}

// Hàm lấy Animation theo tên
Animation* AnimationSet::getAnimation(string name)
{
    for (list<Animation>::iterator animation = animations.begin(); animation != animations.end(); animation++)
    {
        Animation* anim = &(*animation);  // Lấy địa chỉ của animation trong danh sách

        // Nếu tên trùng khớp, trả về animation đó
        if (name == anim->name)
        {
            return anim;
        }
    }

    return NULL;  // Trả về NULL nếu không tìm thấy
}

// Hàm tải tập hợp animation từ file .fdset
void AnimationSet::loadAnimationSet(string fileName, list<DataGroupType>& groupTypes, bool setColourKey, int transparentPixelIndex, bool createWhiteTexture)
{
    ifstream file;  // Đối tượng file để đọc
    string resPath = getResourcePath();  // Lấy đường dẫn tài nguyên
    file.open(resPath + fileName);  // Mở file

    if (file.good())  // Nếu file mở thành công
    {
        getline(file, imageName);  // Đọc tên file ảnh từ dòng đầu tiên

        if (setColourKey)  // Nếu cần thiết lập khóa màu (color keying) cho transparency
        {
            SDL_Surface* spriteSurface = loadSurface(resPath + imageName, Globals::renderer);  // Tải ảnh dưới dạng surface

            // Lấy pixel trong bảng màu để làm trong suốt
            SDL_Color* transparentPixel = &spriteSurface->format->palette->colors[transparentPixelIndex];
            SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));

            spriteSheet = convertSurfaceToTexture(spriteSurface, Globals::renderer, false);  // Chuyển surface thành texture

            if (createWhiteTexture)  // Nếu cần tạo phiên bản trắng của sprite sheet
            {
                SDL_Surface* whiteSurface = loadSurface(resPath + "allwhite.png", Globals::renderer);  // Tải surface trắng
                surfacePaletteSwap(spriteSurface, whiteSurface);  // Hoán đổi bảng màu sang trắng
                SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));
                whiteSpriteSheet = convertSurfaceToTexture(spriteSurface, Globals::renderer, false);  // Tạo texture trắng

                cleanup(whiteSurface);  // Giải phóng surface trắng
            }
            else
            {
                whiteSpriteSheet = NULL;  // Đặt whiteSpriteSheet là NULL nếu không tạo
            }

            cleanup(spriteSurface);  // Giải phóng surface gốc
        }
        else
        {
            spriteSheet = loadTexture(resPath + imageName, Globals::renderer);  // Tải trực tiếp texture nếu không cần color key
        }

        string buffer;
        getline(file, buffer);  // Đọc dòng chứa số lượng animation
        stringstream ss;        // Tạo luồng chuỗi để phân tích
        buffer = Globals::clipOffDataHeader(buffer);  // Loại bỏ tiêu đề (ví dụ: "animations: ")
        ss << buffer;
        int numberOfAnimations;
        ss >> numberOfAnimations;  // Đọc số lượng animation

        // Tải từng animation
        for (int i = 0; i < numberOfAnimations; i++)
        {
            Animation newAnimation;              // Tạo animation mới
            newAnimation.loadAnimation(file, groupTypes);  // Tải dữ liệu animation từ file
            animations.push_back(newAnimation);  // Thêm animation vào danh sách
        }
    }

    file.close();  // Đóng file sau khi đọc xong
}