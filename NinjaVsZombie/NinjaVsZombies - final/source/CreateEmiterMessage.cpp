#include "CreateEmiterMessage.h"


CreateEmiterMessage::CreateEmiterMessage(Entity* myOwner, int mytype)
: Message(MessageID::MSG_CREATE_EMITTER)
{
	m_cseOwner = myOwner;
	m_type = mytype;
}
