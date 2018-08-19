#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"

class Entity;

class CreateEmiterMessage :
	public SGD::Message
{
private:
	Entity* m_cseOwner = nullptr;
	int		m_type;

public:
	CreateEmiterMessage(Entity* myowner, int mytype);
	~CreateEmiterMessage() = default;

	Entity* GetOwner()const			{ return m_cseOwner; }
	int		GetEmiterType()const	{ return m_type; }
};

