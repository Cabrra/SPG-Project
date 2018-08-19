#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "LustWrath.h"

class CreateHeartMessage :
	public SGD::Message
{
private:
	LustWrath* m_pOwner = nullptr;
public:
	CreateHeartMessage(LustWrath* pOwner);
	~CreateHeartMessage();

	LustWrath* GetOwner()const { return m_pOwner; }
};

