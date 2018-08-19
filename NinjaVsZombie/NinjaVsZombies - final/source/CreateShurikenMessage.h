/***************************************************************
|	File:		CreateShurikenMessage.h
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	CreateShurikenMessage class creates new Shuriken entity
***************************************************************/

#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "ThrowingNinja.h"

class CreateShurikenMessage:
	public SGD::Message
{
private:
	ThrowingNinja* m_cbmOwner = nullptr;
public:
	CreateShurikenMessage(ThrowingNinja* _owner);
	~CreateShurikenMessage();

	ThrowingNinja* GetOwner()const { return m_cbmOwner; }
};

