#pragma once
#include "PickUp.h"
class PowerUp :
	public PickUp
{
public:
	enum PowerUpType {
		PU_ATTACK,
		PU_DEFFENSE, PU_STEALTH,
		PU_NOISE
	};

	PowerUp();
	~PowerUp();
};

