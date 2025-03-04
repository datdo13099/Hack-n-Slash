#include "animation.h"

// Hàm khởi tạo Animation với tên
Animation::Animation(string name)
{
    this->name = name;  // Gán tên cho animation
}

// Hàm lấy số thứ tự khung hình tiếp theo
int Animation::getNextFrameNumber(int frameNumber)
{
    if (frameNumber + 1 < frames.size())  // Nếu chưa phải khung cuối
    {
        return frameNumber + 1;  // Trả về số thứ tự khung tiếp theo
    }
    else
    {
        return 0;  // Quay lại khung đầu tiên nếu đã ở khung cuối
    }
}

// Hàm lấy khung hình tiếp theo dựa trên khung hiện tại
Frame* Animation::getNextFrame(Frame* frame)
{
    return getFrame(getNextFrameNumber(frame->frameNumber));  // Lấy khung hình tiếp theo dựa trên số thứ tự
}

// Hàm lấy số thứ tự khung hình cuối cùng
int Animation::getEndFrameNumber()
{
    return frames.size() - 1;  // Trả về chỉ số của khung hình cuối (size - 1)
}

// Hàm lấy khung hình theo số thứ tự
Frame* Animation::getFrame(int frameNumber)
{
    if (frames.size() == 0)  // Nếu danh sách khung hình rỗng
    {
        return NULL;  // Trả về NULL
    }

    list<Frame>::iterator i = frames.begin();  // Con trỏ iterator trỏ tới khung hình đầu tiên
    int counter = 0;

    // Di chuyển iterator đến khung hình mong muốn
    for (counter = 0; counter < frameNumber && counter < frames.size() - 1; counter++)
    {
        i++;  // Chuyển iterator sang khung tiếp theo
    }

    Frame* frame = &(*i);  // Lấy địa chỉ của khung hình mà iterator đang trỏ tới
    // *i: Lấy khung hình tại vị trí của iterator (de-referencing)
    // &: Lấy địa chỉ của khung hình đó (referencing)

    return frame;  // Trả về con trỏ tới khung hình
}

// Hàm tải animation từ file
void Animation::loadAnimation(ifstream& file, list<DataGroupType>& groupTypes)
{
    getline(file, name);  // Đọc tên animation từ dòng đầu tiên

    string buffer;
    getline(file, buffer);  // Đọc dòng chứa số lượng khung hình
    stringstream ss;        // Tạo luồng chuỗi để phân tích
    buffer = Globals::clipOffDataHeader(buffer);  // Loại bỏ tiêu đề (ví dụ: "frames: ")
    ss << buffer;
    int numberOfFrames;
    ss >> numberOfFrames;  // Đọc số lượng khung hình

    // Tải từng khung hình
    for (int i = 0; i < numberOfFrames; i++)
    {
        Frame newFrame;              // Tạo khung hình mới
        newFrame.loadFrame(file, groupTypes);  // Tải dữ liệu khung hình từ file
        frames.push_back(newFrame);  // Thêm khung hình vào danh sách
    }
}