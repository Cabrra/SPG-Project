#include "Particle.h"


Particle::Particle(float cLife, SGD::Point posi, float colorCA, float colorCR, float colorCG, float colorCB,
	float scalC, float rotaC, float velCX, float velCY)
{
	m_currLife				= cLife;
	m_currPosition			= posi;
	m_colorCurrA			= colorCA;
	m_colorCurrR			= colorCR;
	m_colorCurrG			= colorCG;
	m_colorCurrB			= colorCB;
	m_scaleCurr				= scalC;
	m_rotationCurr			= rotaC;
	m_velocityCurrX			= velCX;
	m_velocityCurrY			= velCY;
}


Particle::~Particle()
{
}

void Particle::SetLife(float newLif)
{
	m_currLife = newLif;
}

void Particle::SetPosition(SGD::Point newPosition)
{
	m_currPosition = newPosition;
}

void Particle::SetColorA(float newA)
{
	m_colorCurrA = newA;
}

void Particle::SetColorR(float newR)
{
	m_colorCurrR = newR;
}

void Particle::SetColorG(float newG)
{
	m_colorCurrG = newG;
}

void Particle::SetColorB(float newB)
{
	m_colorCurrB = newB;
}

void Particle::SetScale(float newScal)
{
	m_scaleCurr = newScal;
}

void Particle::SetRotation(float newRot)
{
	m_rotationCurr = newRot;
}

void Particle::SetVelocityX(float newVelX)
{
	m_velocityCurrX = newVelX;
}

void Particle::SetVelocityY(float newVelY)
{
	m_velocityCurrY = newVelY;
}

void	Particle::SetchangeVelocityX(float newChangeVelX)
{
	m_ChangeVelocityX = newChangeVelX;
}

void	Particle::SetchangeVelocityY(float newChangeVelY)
{
	m_ChangeVelocityY = newChangeVelY;
}

void Particle::SetOriginal(float newOri)
{
	m_Original = newOri;
}


//********************************* | GETTERS | ****************************************

float Particle::GetLife()const
{
	return m_currLife;
}

SGD::Point Particle::GetPosition()const
{
	return m_currPosition;
}

float Particle::GetColorA()const
{
	return m_colorCurrA;
}

float Particle::GetColorR()const
{
	return m_colorCurrR;
}

float Particle::GetColorG()const
{
	return m_colorCurrG;
}

float Particle::GetColorB()const
{
	return m_colorCurrB;
}

float Particle::GetScale()const
{
	return m_scaleCurr;
}

float Particle::GetRotation()const
{
	return m_rotationCurr;
}

float Particle::GetVelocityX()const
{
	return m_velocityCurrX;
}

float Particle::GetVelocityY()const
{
	return m_velocityCurrY;
}

float Particle::GetchangeVelocityX()const	
{
	return m_ChangeVelocityX;
}

float Particle::GetchangeVelocityY()const	
{
	return m_ChangeVelocityY;
}

float Particle::GetOriginal()const
{
	return m_Original;
}