/***************************************************************
|	File:		CreateArrowMessage.h
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	CreateArrowMessage class creates new Arrow entity
***************************************************************/

#include "CreateArrowMessage.h"


CreateArrowMessage::CreateArrowMessage(Player* _owner, int myType)
: Message(MessageID::MSG_CREATE_ARROW)
{
	if (_owner){
		m_cbmOwner = _owner;
		//m_cbmOwner->AddRef();
	}
	m_Type = myType;
}


CreateArrowMessage::~CreateArrowMessage()
{
}
