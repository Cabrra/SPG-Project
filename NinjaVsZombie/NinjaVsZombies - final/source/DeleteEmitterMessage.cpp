#include "DeleteEmitterMessage.h"


DeleteEmitterMessage::DeleteEmitterMessage(Entity* myOwner)
: Message(MessageID::MSG_DELETE_EMITTER)
{
	m_deEmiter = myOwner;
}