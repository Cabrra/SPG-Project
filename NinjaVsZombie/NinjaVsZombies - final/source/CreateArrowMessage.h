/***************************************************************
|	File:		CreateArrowMessage.h
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	CreateArrowMessage class creates new Arrow entity
***************************************************************/

#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "Player.h"

class CreateArrowMessage :
	public SGD::Message
{
private:
	Player* m_cbmOwner = nullptr;
	int m_Type = 0;
public:
	CreateArrowMessage(Player* _owner, int myType);
	~CreateArrowMessage();
	int		GetType(void) const	{ return m_Type; }

	Player* GetOwner()const { return m_cbmOwner; }
};