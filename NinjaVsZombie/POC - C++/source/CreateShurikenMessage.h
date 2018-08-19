/***************************************************************
|	File:		CreateShurikenMessage.h
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	CreateShurikenMessage class creates new Shuriken entity
***************************************************************/

#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "Player.h"

class CreateShurikenMessage:
	public SGD::Message
{
private:
	Player* m_cbmOwner = nullptr;
	int m_Type = 0;
public:
	CreateShurikenMessage(Player* _owner, int myType);
	~CreateShurikenMessage();
	int		GetType(void) const	{ return m_Type; }

	Player* GetOwner()const { return m_cbmOwner; }
};

