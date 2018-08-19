#pragma once

#include "../SGD Wrappers/SGD_Geometry.h" 

class Particle
{
private:
	float					m_Original;
	float					m_currLife;
	SGD::Point				m_currPosition;
	float					m_colorCurrA;
	float					m_colorCurrR;
	float					m_colorCurrG;
	float					m_colorCurrB;
	float					m_scaleCurr;
	float					m_rotationCurr;
	float					m_velocityCurrX;
	float					m_velocityCurrY;

	float					m_ChangeA			= 0;
	float					m_ChangeR			= 0;
	float					m_ChangeG			= 0;
	float					m_ChangeB			= 0;
	float					m_ChangeScale		= 0.0f;
	float					m_ChangeRotation	= 0.0f;
	float					m_ChangeVelocityX	= 0.0f;
	float					m_ChangeVelocityY	= 0.0f;

public:
	Particle(float cLife = 0.0f, SGD::Point posi = {}, float colorCA = 0.0f, float colorCR = 0.0f, float colorCG = 0.0f, float colorCB = 0.0f,
		float scalC = 0.0f, float rotaC = 0.0f, float velCX = 0.0f, float velCY = 0.0f);
	~Particle() = default;

	//********************* | SETTERS | *********************************************

	void			SetLife					(float newLif);
	void			SetPosition				(SGD::Point newPosition);
	void			SetColorA				(float newA);
	void			SetColorR				(float newR);
	void			SetColorG				(float newG);
	void			SetColorB				(float newB);
	void			SetScale				(float newScal);
	void			SetRotation				(float newRot);
	void			SetVelocityX			(float newVelX);
	void			SetVelocityY			(float newVelY);

	void			SetOriginal				(float newOri);

	void			SetchangeA				(float newChangeA);
	void			SetchangeR				(float newChangeR);
	void			SetchangeG				(float newChangeG);
	void			SetchangeB				(float newChangeB);
	void			SetchangeScale			(float newChangeSca);
	void			SetchangeRotation		(float newChangeRot);
	void			SetchangeVelocityX		(float newChangeVelX);
	void			SetchangeVelocityY		(float newChangeVelY);

	//********************* | GETTERS | *********************************************

	float					GetLife					()const;
	SGD::Point				GetPosition				()const;
	float					GetColorA				()const;
	float					GetColorR				()const;
	float					GetColorG				()const;
	float					GetColorB				()const;
	float					GetScale				()const;
	float					GetRotation				()const;
	float					GetVelocityX			()const;
	float					GetVelocityY			()const;

	float					GetOriginal				()const;
				
	//float					GetchangeA				()const;
	//float					GetchangeR				()const;
	//float					GetchangeG				()const;
	//float					GetchangeB				()const;
	//float					GetchangeScale			()const;
	float					GetchangeRotation		()const;
	float					GetchangeVelocityX		()const;
	float					GetchangeVelocityY		()const;
};

