#ifndef ANIMATION
#define ANIMATION

#include "frame.h"

using namespace std;

class Animation 
{
public:
	string name; // tên animation
	list<Frame> frames; // danh sách các frame trong animation

	Animation(string name = ""); // constructor, mặc định tên rỗng

	/**
	 * lấy số thứ tự frame tiếp theo trong danh sách
	 * @param frameNumber số thứ tự frame hiện tại
	 * @return số thứ tự frame tiếp theo
	 */
	int getNextFrameNumber(int frameNumber);

	/**
	 * lấy con trỏ đến frame tiếp theo trong animation
	 * @param frame frame hiện tại
	 * @return con trỏ đến frame tiếp theo
	 */
	Frame* getNextFrame(Frame* frame);

	/**
	 * lấy số thứ tự frame cuối cùng trong danh sách
	 * @return số thứ tự frame cuối cùng
	 */
	int getEndFrameNumber();

	/**
	 * lấy con trỏ đến frame theo số thứ tự
	 * @param frameNumber số thứ tự frame cần lấy
	 * @return con trỏ đến frame tương ứng hoặc nullptr nếu không tồn tại
	 */
	Frame* getFrame(int frameNumber);

	/**
	 * tải animation từ file
	 * @param file luồng file đầu vào
	 * @param groupTypes danh sách các loại dữ liệu trong animation
	 */
	void loadAnimation(ifstream& file, list<DataGroupType>& groupTypes);
};

#endif