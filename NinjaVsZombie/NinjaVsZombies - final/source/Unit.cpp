#include "Unit.h"
#include "Emiter.h"

#include "GamePlayState.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "Room.h"
#include "Map.h"
#include "GluttonyGreed.h"

/*virtual*/ void	Unit::Update(float elapsedTime)		/*override;*/
{

}

/*virtual*/ void	Unit::Render(void)					/*override;*/
{

}

/*virtual*/ void	Unit::HandleCollision(const IEntity* pOther)	/*override;*/
{

}

void Unit::SetCurrHP(int newCurrHp)
{
	m_currHealth = newCurrHp;
	if (m_currHealth < 0)
		m_currHealth = 0;
	if (m_currHealth > m_maxHealth)
		m_currHealth = m_maxHealth;
}

bool	Unit::CheckCollision(SGD::Point point)
{
	SGD::Rectangle rect = Game::GetInstance()->GetWorldCamera();
	std::vector<Room*>	rooms = GamePlayState::GetInstance()->GetLevelManager()->GetRooms();
	bool TileCollision = false;
	for (size_t i = 0; i < rooms.size(); i++)
	{
		if (rooms[i]->IsPlayerHere())
		{
			TileCollision = rooms[i]->CheckCollision(point, this);
			break;
		}
	}
	if (TileCollision == false && rect.IsPointInRectangle(point)
		&& !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_BOX_MOVABLE)
		&& !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_BOX_IMMOVABLE)
		&& !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_DOOR)
		&& !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_PLAYER))
		return true;
	return false;
}

bool	Unit::CheckBossCollision(SGD::Point point)
{
	SGD::Rectangle rect = Game::GetInstance()->GetWorldCamera();
	std::vector<Room*>	rooms = GamePlayState::GetInstance()->GetLevelManager()->GetRooms();
	for (size_t i = 0; i < rooms.size(); i++)
	{
		if (rooms[i]->IsPlayerHere() && !rooms[i]->CheckCollision(point, this) && rect.IsPointInRectangle(point)
			&& !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_BOX_MOVABLE)
			&& !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_BOX_IMMOVABLE)
			//&& (!GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_ENEMY)
			/*&& !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_DOOR)*/
			//|| !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_PLAYER)
			//)
			)
		{
		
			return true;
		}
	}
	GluttonyGreed* glut = (GluttonyGreed*) this;
	glut->SetMyState(Enemy::AI_IDLE);
	glut->SetPath(true);
	return false;
}