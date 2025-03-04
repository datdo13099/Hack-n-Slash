#ifndef ANIMATIONSET
#define ANIMATIONSET

#include "animation.h"

using namespace std;

class AnimationSet 
{
public:
	string imageName; // tên file ảnh spritesheet
	SDL_Texture* spriteSheet; // chứa ảnh spritesheet cho tất cả animation
	SDL_Texture* whiteSpriteSheet = NULL; // spritesheet dùng để hiển thị hiệu ứng bị thương
	list<Animation> animations; // danh sách các animation

	~AnimationSet(); // hàm hủy, giải phóng bộ nhớ động (textures)

	Animation* getAnimation(string name); // lấy animation theo tên

	void loadAnimationSet(string fileName, list<DataGroupType>& groupTypes,
		bool setColourKey = false, int transparentPixelIndex = 0, bool createWhiteTexture = false);
	// tải animation từ file, hỗ trợ đặt màu trong suốt và tạo texture trắng
};

#endif