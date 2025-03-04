#include "res_path.h"

/*
* lấy đường dẫn tài nguyên cho các tài nguyên nằm trong res/subDir
* giả định rằng thư mục dự án có cấu trúc như sau:
* bin/
*  thực thi
* res/
*  Lesson1/
*  Lesson2/
*
* đường dẫn trả về sẽ là Lessons/res/subDir
*/
std::string getResourcePath(const std::string& subDir)
{
    // cần chọn dấu phân cách đường dẫn phù hợp tùy theo nền tảng
    // vì windows sử dụng dấu phân cách khác so với hầu hết hệ thống khác
#ifdef _WIN32
    const char PATH_SEP = '\\';
#else
    const char PATH_SEP = '/';
#endif
    // biến này sẽ chứa đường dẫn cơ sở: Lessons/res/
    // đặt thời gian tồn tại static để chỉ cần gọi SDL_GetBasePath một lần
    // để lấy đường dẫn thực thi
    static std::string baseRes;
    if (baseRes.empty())
    {
        // SDL_GetBasePath sẽ trả về NULL nếu có lỗi xảy ra khi lấy đường dẫn
        char* basePath = SDL_GetBasePath();
        if (basePath)
        {
            baseRes = basePath;
            SDL_free(basePath);
        }
        else
        {
            std::cerr << "Lỗi khi lấy đường dẫn tài nguyên: " << SDL_GetError() << std::endl;
            return "";
        }
        // thay thế bin/ cuối cùng bằng res/ để có đường dẫn tài nguyên
        size_t pos = baseRes.rfind("bin");
        baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
    }
    // nếu cần đường dẫn của một thư mục con cụ thể trong thư mục tài nguyên
    // nối nó vào đường dẫn cơ sở. ví dụ: Lessons/res/Lesson0
    return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}