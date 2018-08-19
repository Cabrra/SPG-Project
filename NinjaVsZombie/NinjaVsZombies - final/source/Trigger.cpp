#include "Trigger.h"
#include "Tile.h"
#include "Entity.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

Trigger::Trigger()
{
}


Trigger::~Trigger()
{
}

void Trigger::CheckCollision(const IEntity* pOther)
{
	for (unsigned int x = 0; x < m_vTiles.size(); x++)
	{

		if (pOther->GetRect().IsIntersecting(m_vTiles[x]->GetRect()) && m_vTiles[x]->type == Tile::m_eTiletype::trigger)
			HandleCollision(m_pObjToTrigger);
	}
}

void Trigger::HandleCollision(const IEntity* pOther)
{
	//if (pOther->GetType() == Entity::EntityType::ENT_PLAYER)
	{
		if (m_bActivated == false)
		{
			m_bActivated = true;

			SGD::GraphicsManager::GetInstance()->DrawString("Triggered event",
				SGD::Point(400, 400), SGD::Color(255, 255, 255));
		}
	}
}
