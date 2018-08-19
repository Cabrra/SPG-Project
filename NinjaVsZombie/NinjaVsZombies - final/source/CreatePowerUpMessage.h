#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "MessageID.h"

class CreatePowerUpMessage :
	public SGD::Message
{

private:
	//*******************************************************************
	// members:
	int				m_PowerType = 0;
	SGD::Point		m_myPoint = { 0, 0 };
	float			m_myLevel = 0;
	

public:
	CreatePowerUpMessage(int type, SGD::Point pos, float level);
	~CreatePowerUpMessage() = default;

	int				GetPowerType()const		{ return m_PowerType; }
	SGD::Point		GetPoint()const			{ return m_myPoint; }
	float				GetLevel()const			{ return m_myLevel; }

	void		SetPowerType(int newType)		{ m_PowerType = newType; }
	void		SetPoint(SGD::Point newPos)			{ m_myPoint = newPos; }
	void		SetLevel(float newLevel)				{ m_myLevel = newLevel; }
};