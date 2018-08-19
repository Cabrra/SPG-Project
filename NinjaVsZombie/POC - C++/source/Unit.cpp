#include "Unit.h"

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