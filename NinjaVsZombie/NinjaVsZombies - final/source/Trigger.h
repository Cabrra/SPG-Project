#pragma once
#include <vector>
#include "Entity.h"

class Tile;

class Trigger
{
protected:
	std::vector<Tile*> m_vTiles;
	Entity* m_pObjToTrigger = nullptr;
	bool m_bActivated = false;

public:
	Trigger();
	virtual ~Trigger();

	std::vector<Tile*> GetTiles()const { return m_vTiles; }
	Entity* GetObjToTrigger()const { return m_pObjToTrigger; }
	bool IsActivated()const { return m_bActivated; }

	void SetObjToTrigger(Entity* obj) {m_pObjToTrigger = obj; }
	void SetActivated(bool active) {m_bActivated = active; }

	void CheckCollision(const IEntity* pOther);
	virtual void HandleCollision(const IEntity* pOther);


};

