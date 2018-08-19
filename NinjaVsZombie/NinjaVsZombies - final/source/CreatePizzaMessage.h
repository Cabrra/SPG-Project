#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "Player.h"
class CreatePizzaMessage :
	public SGD::Message
{
private:
	SGD::Point		m_myPoint = { 0, 0 };
public:
	CreatePizzaMessage(SGD::Point pos);
	~CreatePizzaMessage() = default;

	SGD::Point	GetPoint()const		{ return m_myPoint; }
};

