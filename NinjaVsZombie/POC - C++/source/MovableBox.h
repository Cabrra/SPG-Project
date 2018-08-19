#pragma once

#include "Puzzle.h"
class MovableBox:
	public Puzzle
{
public:
	MovableBox();
	~MovableBox();

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_MOVABLE; }
	virtual void	HandleCollision(const IEntity* pOther)	override;
};