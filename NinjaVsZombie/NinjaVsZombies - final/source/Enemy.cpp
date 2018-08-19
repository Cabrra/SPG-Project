#include "Enemy.h"


Enemy::Enemy() 
{


}


Enemy::~Enemy()
{
	SetTarget(nullptr);
	m_Path.clear();
}

void Enemy::Update(float elapsedTime)
{

}
void Enemy::Render(void)
{

}
void Enemy::HandleCollision(const IEntity* pOther)
{

}

void Enemy::SetTarget(Entity* newTarget)
{
	if (m_Target != nullptr)
		m_Target->Release();

	m_Target = newTarget;

	if (m_Target != nullptr)
		m_Target->AddRef();
}