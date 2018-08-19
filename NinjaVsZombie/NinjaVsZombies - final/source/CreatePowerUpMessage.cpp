#include "CreatePowerUpMessage.h"
#include "MessageID.h"


CreatePowerUpMessage::CreatePowerUpMessage(int type, SGD::Point pos, float level)
: Message(MessageID::MSG_CREATE_POWERUP)
{
	m_PowerType = type;
	m_myPoint = pos;
	m_myLevel = level;
}
