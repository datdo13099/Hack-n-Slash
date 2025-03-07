#ifndef GLOB
#define GLOB

#include <cstdlib>
#include "livingEntity.h"

class Glob : public LivingEntity
{
public:
	static const string GLOB_ANIM_UP;
	static const string GLOB_ANIM_DOWN;
	static const string GLOB_ANIM_LEFT;
	static const string GLOB_ANIM_RIGHT;
	static const string GLOB_ANIM_IDLE_UP;
	static const string GLOB_ANIM_IDLE_DOWN;
	static const string GLOB_ANIM_IDLE_LEFT;
	static const string GLOB_ANIM_IDLE_RIGHT;
	static const string GLOB_ANIM_ATTACK_UP;
	static const string GLOB_ANIM_ATTACK_DOWN;
	static const string GLOB_ANIM_ATTACK_LEFT;
	static const string GLOB_ANIM_ATTACK_RIGHT;
	static const string GLOB_ANIM_TELEGRAPH_UP;
	static const string GLOB_ANIM_TELEGRAPH_DOWN;
	static const string GLOB_ANIM_TELEGRAPH_LEFT;
	static const string GLOB_ANIM_TELEGRAPH_RIGHT;
	static const string GLOB_ANIM_DIE;

	static const int GLOB_STATE_IDLE;       // trạng thái đứng yên
	static const int GLOB_STATE_MOVE;       // trạng thái di chuyển
	static const int GLOB_STATE_ATTACK;     // trạng thái tấn công
	static const int GLOB_STATE_TELEGRAPH;  // trạng thái chuẩn bị tấn công
	static const int GLOB_STATE_DEAD;       // trạng thái chết

	static const int GLOB_AI_NORMAL;  // trạng thái AI bình thường
	static const int GLOB_AI_CHASE;   // trạng thái AI đuổi theo mục tiêu

	static int globsKilled;  // số lượng globs đã bị tiêu diệt

	// khi biến này bằng 0, AI sẽ chọn thực hiện một hành động
	float thinkTimer = 0;
	// đối tượng mà AI sẽ đuổi theo
	LivingEntity* target = NULL;

	int aiState = GLOB_AI_NORMAL;  // trạng thái hiện tại của AI

	Glob(AnimationSet* animSet);
	void update();    // cập nhật trạng thái
	void think();     // xử lý tư duy AI
	void telegraph(); // thực hiện hành động telegraph
	void attack();
	void die();
	void findNearestTarget();
	void changeAnimation(int newState, bool resetFrameToBeginning);
	void updateAnimation();
	void updateDamages();
};

#endif