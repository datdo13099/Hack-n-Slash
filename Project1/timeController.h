#ifndef TIMECONTROLLER
#define TIMECONTROLLER
#include <iostream>
#include <SDL.h>

class TimeController
{
public:
	//gia tri tham chieu
	static const int PLAY_STATE, PAUSE_STATE;

	int timeState;
	Uint32 lastUpdate; //kiem tra xem lan truoc duoc bao nhieu tic tac
	float dT; //thời gian delta, tính bằng giây (1= 1 giây, 0,5 là nửa giây). Thời gian kể từ khi khung hình cuối cùng được hiển thị trên màn hình
	
	TimeController();
	void updateTime(); //update lastUpdate va dT
	void pause();
	void resume();
	void reset();

	static TimeController timeController;
}; 

#endif