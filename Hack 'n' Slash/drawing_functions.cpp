#include "drawing_functions.h"

/**
 * Tải một ảnh vào texture trên thiết bị render
 * @param file Đường dẫn file ảnh cần tải
 * @param ren Renderer để tải texture lên
 * @return Texture đã tải, hoặc nullptr nếu có lỗi xảy ra
 */
SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* ren)
{
    SDL_Texture* texture = IMG_LoadTexture(ren, file.c_str());  // Tải ảnh thành texture
    if (texture == nullptr)  // Nếu tải thất bại
    {
        cout << "Lỗi LoadTexture" << endl;  // In thông báo lỗi
    }
    return texture;  // Trả về texture
}

/**
 * Tải ảnh thành một surface, thường dùng khi cần hoán đổi bảng màu
 */
SDL_Surface* loadSurface(const std::string& file, SDL_Renderer* ren)
{
    SDL_Surface* surface = IMG_Load(file.c_str());  // Tải ảnh thành surface
    if (surface == nullptr)  // Nếu tải thất bại
    {
        cout << "Lỗi LoadSurface" << endl;  // In thông báo lỗi
    }
    return surface;  // Trả về surface
}

/**
 * Sao chép bảng màu từ một surface sang surface khác
 */
void surfacePaletteSwap(SDL_Surface* surface, SDL_Surface* palette)
{
    // Gán bảng màu của palette sang surface
    SDL_SetPaletteColors(surface->format->palette, palette->format->palette->colors, 0, palette->format->palette->ncolors);
}

/**
 * Chuyển đổi surface thành texture và tùy chọn xóa surface
 */
SDL_Texture* convertSurfaceToTexture(SDL_Surface* surface, SDL_Renderer* ren, bool cleanSurface)
{
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);  // Tạo texture từ surface
    if (cleanSurface)  // Nếu yêu cầu xóa surface
    {
        cleanup(surface);  // Giải phóng surface
    }
    return texture;  // Trả về texture
}

/**
 * Vẽ một SDL_Texture lên SDL_Renderer tại một hình chữ nhật đích
 * Có thể lấy một phần của texture nếu muốn
 * @param tex Texture nguồn để vẽ
 * @param ren Renderer để vẽ lên
 * @param dst Hình chữ nhật đích để vẽ texture
 * @param clip Phần của texture để vẽ (mặc định nullptr sẽ vẽ toàn bộ texture)
 */
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect dst, SDL_Rect* clip)
{
    SDL_RenderCopy(ren, tex, clip, &dst);  // Sao chép texture lên renderer
}

/**
 * Vẽ một SDL_Texture lên SDL_Renderer tại tọa độ x, y, giữ nguyên kích thước texture
 * Nếu có clip, dùng kích thước của clip thay vì kích thước texture
 * @param tex Texture nguồn để vẽ
 * @param ren Renderer để vẽ lên
 * @param x Tọa độ x để vẽ
 * @param y Tọa độ y để vẽ
 * @param clip Phần của texture để vẽ (mặc định nullptr sẽ vẽ toàn bộ texture)
 */
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, SDL_Rect* clip)
{
    SDL_Rect dst;  // Hình chữ nhật đích
    dst.x = x;     // Gán tọa độ x
    dst.y = y;     // Gán tọa độ y
    if (clip != nullptr)  // Nếu có clip
    {
        dst.w = clip->w;  // Dùng chiều rộng của clip
        dst.h = clip->h;  // Dùng chiều cao của clip
    }
    else  // Nếu không có clip
    {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);  // Lấy kích thước từ texture
    }
    renderTexture(tex, ren, dst, clip);  // Gọi hàm vẽ với thông tin đã thiết lập
}

/**
 * Chuyển văn bản thành texture để vẽ
 * @param message Văn bản muốn hiển thị
 * @param fontFile File font để render văn bản
 * @param color Màu sắc của văn bản
 * @param fontSize Kích thước font
 * @param renderer Renderer để tải texture
 * @return Texture chứa văn bản đã render, hoặc nullptr nếu có lỗi
 */
SDL_Texture* renderText(const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer)
{
    // Mở font (CẢNH BÁO: Nên lưu font bên ngoài hàm này để tránh ảnh hưởng hiệu suất)
    TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr)  // Nếu mở font thất bại
    {
        cout << "Lỗi TTF_OpenFont" << endl;
        return nullptr;
    }

    // Render văn bản thành surface
    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
    if (surf == nullptr)  // Nếu render thất bại
    {
        TTF_CloseFont(font);  // Đóng font
        cout << "Lỗi TTF_RenderText" << endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);  // Chuyển surface thành texture
    if (texture == nullptr)  // Nếu tạo texture thất bại
    {
        cout << "Lỗi CreateTexture" << endl;
    }

    // Giải phóng tài nguyên
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;  // Trả về texture
}

/**
 * Chuyển văn bản thành texture để vẽ (dùng font đã có sẵn)
 * @param message Văn bản muốn hiển thị
 * @param font Font để render văn bản
 * @param color Màu sắc của văn bản
 * @param renderer Renderer để tải texture
 * @return Texture chứa văn bản đã render, hoặc nullptr nếu có lỗi
 */
SDL_Texture* renderText(const std::string& message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer)
{
    if (font == nullptr)  // Nếu font không hợp lệ
    {
        cout << "Lỗi TTF_OpenFont" << endl;
        return nullptr;
    }

    // Render văn bản thành surface
    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
    if (surf == nullptr)  // Nếu render thất bại
    {
        cout << "Lỗi TTF_RenderText" << endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);  // Chuyển surface thành texture
    if (texture == nullptr)  // Nếu tạo texture thất bại
    {
        cout << "Lỗi CreateTexture" << endl;
    }

    // Giải phóng surface (không đóng font vì được truyền từ ngoài)
    SDL_FreeSurface(surf);
    return texture;  // Trả về texture
}

/**
 * Lưu ảnh chụp màn hình dưới dạng BMP
 * @param filepath Đường dẫn file để lưu
 * @param SDLWindow Cửa sổ SDL
 * @param SDLRenderer Renderer SDL
 * @return True nếu lưu thành công, False nếu thất bại
 */
bool saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer)
{
    SDL_Surface* saveSurface = NULL;
    SDL_Surface* infoSurface = NULL;
    infoSurface = SDL_GetWindowSurface(SDLWindow);  // Lấy thông tin surface từ cửa sổ
    if (infoSurface == NULL)  // Nếu lấy thất bại
    {
        std::cerr << "Không thể tạo info surface từ cửa sổ trong saveScreenshotBMP, SDL_GetError() - " << SDL_GetError() << "\n";
    }
    else
    {
        // Cấp phát bộ nhớ cho dữ liệu pixel
        unsigned char* pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
        if (pixels == 0)  // Nếu cấp phát thất bại
        {
            std::cerr << "Không thể cấp phát bộ nhớ cho dữ liệu pixel ảnh chụp màn hình!\n";
            return false;
        }
        else
        {
            // Đọc dữ liệu pixel từ renderer
            if (SDL_RenderReadPixels(SDLRenderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0)
            {
                std::cerr << "Không thể đọc dữ liệu pixel từ SDL_Renderer. SDL_GetError() - " << SDL_GetError() << "\n";
                delete[] pixels;
                return false;
            }
            else
            {
                // Tạo surface từ dữ liệu pixel
                saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
                if (saveSurface == NULL)  // Nếu tạo surface thất bại
                {
                    std::cerr << "Không thể tạo SDL_Surface từ dữ liệu pixel renderer. SDL_GetError() - " << SDL_GetError() << "\n";
                    delete[] pixels;
                    return false;
                }
                SDL_SaveBMP(saveSurface, filepath.c_str());  // Lưu surface thành file BMP
                SDL_FreeSurface(saveSurface);  // Giải phóng surface
            }
            delete[] pixels;  // Giải phóng bộ nhớ pixel
        }
        SDL_FreeSurface(infoSurface);  // Giải phóng infoSurface
    }
    return true;  // Trả về true nếu lưu thành công
}