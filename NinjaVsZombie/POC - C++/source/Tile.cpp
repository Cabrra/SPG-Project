#include "Tile.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "TileSet.h"
#include "Entity.h"

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
	if (pOther->GetType() == Entity::EntityType::ENT_PLAYER)
		this->HandleTrigger();
}

void Tile::HandleTrigger()
{
	//if (isTriggered == false)
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