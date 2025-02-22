#include "animation.h" 

Animation::Animation(string name)
{
	this->name = name;
}

int Animation::getNextFrameNumber(int frameNumber)
{
	if (frameNumber + 1 < frames.size())
		return frameNumber + 1;
	else 
		return 0;
}
Frame* Animation::getNextFrame(Frame* frame)
{
	return getFrame(getNextFrameNumber(frame->frameNumber));
}
int Animation::getEndFrameNumber()
{
	return frames.size() - 1;
}
Frame* Animation::getFrame(int frameNumber)
{
	if (frames.size() == 0)
		return NULL;

	list<Frame>::iterator i = frames.begin(); //tro iterator den khung hinh dau tien trong danh sach khung hinh

	int counter = 0;

	for (counter = 0; counter < frameNumber && counter < frames.size() - 1; counter++)
	{
		i++; //tro den khung hinh tiep theo trong danh sach
	}

	Frame* frame = &(*i); //lam cho khung hinh tro den khung hinh ben trong danh sach ma vong lap tro toi
						//*i: lay khung hinh o vi tri thu i trong danh sach (bo tham chieu)
						//& = lay dia chi bo nho cua bat ky thu gi ben phai (tham chieu)
	return frame;
}

void Animation::loadAnimation(ifstream& file, list<DataGroupType>& groupTypes) 
{
	getline(file, name);
	string buffer;
	getline(file, buffer);
	stringstream ss;
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	int numberOfFrames;
	ss >> numberOfFrames;
	for (int i = 0; i < numberOfFrames; i++) 
	{
		Frame newFrame;
		newFrame.loadFrame(file, groupTypes);
		frames.push_back(newFrame);
	}


}