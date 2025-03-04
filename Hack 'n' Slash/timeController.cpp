#include "timeController.h"

const int TimeController::PLAY_STATE = 0, TimeController::PAUSE_STATE = 1;

TimeController::TimeController()
{
    dT = 0;
    lastUpdate = 0;
    timeState = PLAY_STATE;
}

void TimeController::updateTime()
{
    if (timeState == PAUSE_STATE)
    {
        dT = 0;
    }
    else
    {
        Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
        dT = timeDiff / 1000.0; // chuyển mili-giây thành giây, ví dụ: 104 tick = 0.104 giây
    }
    lastUpdate = SDL_GetTicks();
}

void TimeController::pause()
{
    timeState = PAUSE_STATE;
}

void TimeController::resume()
{
    timeState = PLAY_STATE;
}

void TimeController::reset()
{
    dT = 0;
    lastUpdate = SDL_GetTicks();
}

// tạo một đối tượng timeController toàn cục để sử dụng ở bất cứ đâu
TimeController TimeController::timeController;