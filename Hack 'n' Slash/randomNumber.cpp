#include "randomNumber.h"

int getRandomNumber(int mod)
{
    // trả về số ngẫu nhiên trong khoảng từ 0 đến mod
    return rand() % mod;
}
