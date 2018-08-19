#include "Item.h"


Item::Item()
{
}


Item::~Item()
{
}


/*virtual*/ void	Item::Render(void)					/*override;*/
{

}

/*virtual*/ SGD::Rectangle Item::GetRect(void)	const			/*override;*/
{
	return Entity::GetRect();
}

/*virtual*/ void	Item::HandleCollision(const IEntity* pOther)	/*override;*/
{

}