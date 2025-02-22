#ifndef ANIMATION
#define ANIMATION

#include "frame.h"

using namespace std;

class Animation
{
public:
	string name; //ten cua animation
	list<Frame> frames; // danh sang cac khung hinh

	Animation(string name = "");

	int getNextFrameNumber(int frameNumber); //tra ve khung hinh tiep theo trong danh sach
	Frame* getNextFrame(Frame* frame);
	int getEndFrameNumber(); //tra ve so frame cuoi cung
	Frame* getFrame(int frameNumber); //lay khung hinh bang cach su dung so khung hinh

	void loadAnimation(ifstream& file, list<DataGroupType>& groupTypes);
};

#endif 