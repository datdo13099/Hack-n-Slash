#ifndef DRAWINGFUNCS_H
#define DRAWINGFUNCS_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "res_path.h"
#include "cleanup.h"

using namespace std;

/**
 * tải hình ảnh vào texture trên thiết bị render
 * @param file đường dẫn file ảnh
 * @param ren renderer dùng để load texture
 * @return texture đã load hoặc nullptr nếu có lỗi
 */
SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* ren);

/**
 * tải hình ảnh dưới dạng surface, thường dùng khi cần đổi bảng màu
 */
SDL_Surface* loadSurface(const std::string& file, SDL_Renderer* ren);

/**
 * sao chép bảng màu từ surface này sang surface khác
 */
void surfacePaletteSwap(SDL_Surface* surface, SDL_Surface* palette);

/**
 * chuyển surface thành texture, có thể xóa surface sau khi chuyển đổi
 */
SDL_Texture* convertSurfaceToTexture(SDL_Surface* surface, SDL_Renderer* ren, bool cleanSurface = false);

/**
 * vẽ texture lên renderer với vị trí xác định, có thể cắt một phần texture nếu cần
 * @param tex texture nguồn cần vẽ
 * @param ren renderer để vẽ lên
 * @param dst vị trí và kích thước để vẽ texture
 * @param clip phần texture cần cắt, nếu nullptr thì vẽ toàn bộ
 */
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect dst, SDL_Rect* clip = nullptr);

/**
 * vẽ texture lên renderer tại tọa độ x, y, giữ nguyên kích thước gốc của texture
 * nếu có clip thì dùng kích thước của clip thay vì kích thước gốc
 * @param tex texture nguồn cần vẽ
 * @param ren renderer để vẽ lên
 * @param x tọa độ x
 * @param y tọa độ y
 * @param clip phần texture cần cắt, nếu nullptr thì vẽ toàn bộ
 */
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, SDL_Rect* clip = nullptr);

/**
 * render văn bản thành texture để vẽ lên màn hình
 * @param message nội dung văn bản
 * @param fontFile đường dẫn file font chữ
 * @param color màu của chữ
 * @param fontSize kích thước chữ
 * @param renderer renderer để load texture
 * @return texture chứa văn bản đã render, hoặc nullptr nếu có lỗi
 */
SDL_Texture* renderText(const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer);

/**
 * render văn bản thành texture để vẽ lên màn hình
 * @param message nội dung văn bản
 * @param font font đã load sẵn
 * @param color màu chữ
 * @param renderer renderer để load texture
 * @return texture chứa văn bản đã render, hoặc nullptr nếu có lỗi
 */
SDL_Texture* renderText(const std::string& message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer);

/**
 * lưu ảnh chụp màn hình dưới dạng file BMP
 * @param filepath đường dẫn file lưu ảnh
 * @param SDLWindow cửa sổ SDL
 * @param SDLRenderer renderer SDL
 * @return true nếu lưu thành công, false nếu thất bại
 */
bool saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer);

#endif
