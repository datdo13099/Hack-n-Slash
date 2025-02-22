#ifndef ANIMATIONSET
#define ANIMATIONSET

#include "animation.h"

using namespace std;

class AnimationSet
{
public:
	string imageName;
	SDL_Texture* spriteSheet; 
	SDL_Texture* whiteSpriteSheet = NULL;
	list<Animation> animations;

	~AnimationSet();

	Animation* getAnimation(string name);
	void loadAnimationSet(string fileName, list<DataGroupType>& groupTypes, bool setColourKey = false, int transparentPixelIndex = 0, bool createWhiteTexture = false);
};

#endif