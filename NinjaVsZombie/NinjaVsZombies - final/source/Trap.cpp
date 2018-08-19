#include "Trap.h"


Trap::Trap()
{
}


Trap::~Trap()
{
}

void Trap::Update(float elapsedTime)
{
}

void Trap::Render(void)
{

}

SGD::Rectangle Trap::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Trap::HandleCollision(const IEntity* pOther)
{

}