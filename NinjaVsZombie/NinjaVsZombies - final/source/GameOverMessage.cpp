#include "GameOverMessage.h"
#include "MessageID.h"
#include "Entity.h"


GameOverMessage::GameOverMessage(Entity* owner)
: Message(MessageID::MSG_GAME_OVER)
{
	m_Owner = owner;
}