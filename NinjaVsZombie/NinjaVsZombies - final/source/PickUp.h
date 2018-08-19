#pragma once

#include "Unbreakable.h"
class PickUp:
	public Unbreakable
{
public:
	PickUp();
	~PickUp();

	 virtual bool CheckCollision();
};