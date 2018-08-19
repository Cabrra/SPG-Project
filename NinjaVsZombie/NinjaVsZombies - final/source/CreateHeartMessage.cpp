#include "CreateHeartMessage.h"


CreateHeartMessage::CreateHeartMessage(LustWrath* pOwner)
: Message(MessageID::MSG_CREATE_HEART)
{
	if (pOwner)
	{
		m_pOwner = pOwner;
		m_pOwner->AddRef();
	}
}


CreateHeartMessage::~CreateHeartMessage()
{
	m_pOwner->Release();
}
