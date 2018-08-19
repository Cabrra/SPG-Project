#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"

class Entity;

class DeleteEmitterMessage :
	public SGD::Message
{
private:
	Entity* m_deEmiter = nullptr;
public:
	DeleteEmitterMessage(Entity* myOwner);
	~DeleteEmitterMessage() = default;

	Entity* GetMyOwner()const { return m_deEmiter; }

};

