/***************************************************************
|	File:		CreateShurikenMessage.h
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	CreateShurikenMessage class creates new Shuriken entity
***************************************************************/

#include "CreateShurikenMessage.h"


CreateShurikenMessage::CreateShurikenMessage(ThrowingNinja* _owner)
: Message(MessageID::MSG_CREATE_SHURIKEN)
{
	if (_owner){
		m_cbmOwner = _owner;
		m_cbmOwner->AddRef();
	}
}

CreateShurikenMessage::~CreateShurikenMessage()
{
	m_cbmOwner->Release();
}
