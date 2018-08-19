#pragma once

#include "Unit.h"
class Enemy : public Unit
{
public:
	enum AIState{
		AI_IDLE, AI_ATTACK,
		AI_ALERT, AI_CAUTION,
		AI_PATROL
	};

	Enemy();
	~Enemy();
};