#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"

class Entity;
class GameOverMessage : public SGD::Message
{
	Entity*  m_Owner = nullptr;
public:
	GameOverMessage(Entity* owner);
	~GameOverMessage()= default;

	Entity* GetOwner()const { return m_Owner; }
};