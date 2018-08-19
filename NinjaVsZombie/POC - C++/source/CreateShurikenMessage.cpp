/***************************************************************
|	File:		CreateShurikenMessage.h
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	CreateShurikenMessage class creates new Shuriken entity
***************************************************************/

#include "CreateShurikenMessage.h"


CreateShurikenMessage::CreateShurikenMessage(Player* _owner, int myType)
: Message(MessageID::MSG_CREATE_SHURIKEN)
{
	if (_owner){
		m_cbmOwner = _owner;
		//m_cbmOwner->AddRef();
	}
	m_Type = myType;
}

CreateShurikenMessage::~CreateShurikenMessage()
{
}
