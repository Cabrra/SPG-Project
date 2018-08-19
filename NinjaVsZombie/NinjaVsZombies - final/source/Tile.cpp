#include "Tile.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "TileSet.h"
#include "Entity.h"
#include "Player.h"

Tile::Tile()
{
}

Tile::~Tile()
{
}

SGD::Rectangle Tile::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Tile::Update(float elapsedTime)
{
}

void Tile::Render(void)
{
	//do nothing let map render tile
}

void Tile::HandleCollision(const IEntity* pOther)
{
	switch (type)
	{
	case Tile::m_eTiletype::walkable:
		break;

	case Tile::m_eTiletype::collidable:
	{
	}
		break;

	case Tile::m_eTiletype::hole:
	{
			Entity* ply = (Entity*)pOther;
			if (GetRect().IsPointInRectangle(ply->GetRect().ComputeCenter()))
			{
				if (pOther->GetType() == Entity::ENT_PLAYER)
				{
					Player* player = (Player*)pOther;
					if (player->IsHookActive() == false)
						ply->SetFalling(true);
				}
				else
					ply->SetFalling(true);
			}
	}
		break;

	case Tile::m_eTiletype::trigger:
		this->HandleTrigger();
		break;
	default:
		break;
	}
}

void Tile::HandleTrigger()
{
	if (isTriggered == false)
	{
		isTriggered = true;
		SGD::GraphicsManager::GetInstance()->DrawString("Triggered event",
			SGD::Point(400, 400), SGD::Color(255, 255, 255));
	}
}

//leave empty 
void Tile::AddRef(void)
{
}

void Tile::Release(void)
{
}