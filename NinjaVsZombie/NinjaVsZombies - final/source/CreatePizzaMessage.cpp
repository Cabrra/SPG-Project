#include "CreatePizzaMessage.h"


CreatePizzaMessage::CreatePizzaMessage(SGD::Point pos)
: Message(MessageID::MSG_CREATE_PIZZA)
{
	m_myPoint = pos;
}

