#include "ParticleFlyweight.h"


ParticleFlyweight::ParticleFlyweight(float min, float max, float clorSA, float clorSR, float clorSG, float clorSB, float clorEA, float clorER, float clorEG, float clorEB,
	float scaleS , float scaleE, float rotaS, float rotaE, float velSMinX, float velSMaxX, float velEMinX, float velEMaxX, float velSMinY, float velSMaxY, float velEMinY, float velEMaxY)
{
	m_minLife					= min;
	m_maxLife					= max;
	m_colorStartA				= clorSA;
	m_colorStartR				= clorSR;
	m_colorStartG				= clorSG;
	m_colorStartB				= clorSB;
	m_colorEndA					= clorEA;
	m_colorEndR					= clorER;
	m_colorEndG					= clorEG;
	m_colorEndB					= clorEB;
	m_scaleStart				= scaleS;
	m_scaleEnd					= scaleE;
	m_rotationStart				= rotaS;
	m_rotationEnd				= rotaE;
	m_velocityStartMinX			= velSMinX;
	m_velocityStartMaxX			= velSMaxX;
	m_velocityEndMinX			= velEMinX;
	m_velocityEndMaxX			= velEMaxX;
	m_velocityStartMinY			= velSMinY;
	m_velocityStartMaxY			= velSMaxY;
	m_velocityEndMinY			= velEMinY;
	m_velocityEndMaxY			= velEMaxY;
}

//********************* | SETTERS | *********************************************

ParticleFlyweight::~ParticleFlyweight()
{
}

void ParticleFlyweight::SetMinLife(float newMin)
{
	m_minLife = newMin;
}

void ParticleFlyweight::SetMaxLife(float newMax)
{
	m_maxLife = newMax;
}

void ParticleFlyweight::SetColorStartA(float newSA)
{
	m_colorStartA = newSA;
}

void ParticleFlyweight::SetColorStartR(float newSR)
{
	m_colorStartR = newSR;
}

void ParticleFlyweight::SetColorStartG(float newSG)
{
	m_colorStartG = newSG;
}

void ParticleFlyweight::SetColorStartB(float newSB)
{
	m_colorStartB = newSB;
}

void ParticleFlyweight::SetColorEndA(float newEA)
{
	m_colorEndA = newEA;
}

void ParticleFlyweight::SetColorEndR(float newER)
{
	m_colorEndR = newER;
}

void ParticleFlyweight::SetColorEndG(float newEG)
{
	m_colorEndG = newEG;
}

void ParticleFlyweight::SetColorEndB(float newEB)
{
	m_colorEndB = newEB;
}

void ParticleFlyweight::SetScaleStart(float newScalS)
{
	m_scaleStart = newScalS;
}

void ParticleFlyweight::SetScaleEnd(float newScalE)
{
	m_scaleEnd = newScalE;
}

void ParticleFlyweight::SetRotationStart(float newRotS)
{
	m_rotationStart = newRotS;
}

void ParticleFlyweight::SetRotationEnd(float newRotE)
{
	m_rotationEnd = newRotE;
}

void ParticleFlyweight::SetVelocityStartMinX(float newVelSmxX)
{
	m_velocityStartMinX = newVelSmxX;
}

void ParticleFlyweight::SetVelocityStartMaxX(float newVelSmxX)
{
	m_velocityStartMaxX = newVelSmxX;
}

void ParticleFlyweight::SetVelocityEndMinX(float newVelEmnX)
{
	m_velocityEndMinX = newVelEmnX;
}

void ParticleFlyweight::SetVelocityEndMaxX(float newVelEmxX)
{
	m_velocityEndMaxX = newVelEmxX;
}

void ParticleFlyweight::SetVelocityStartMinY(float newVelSmnY)
{
	m_velocityStartMinY = newVelSmnY;
}

void ParticleFlyweight::SetVelocityStartMaxY(float newVelSmxY)
{
	m_velocityStartMaxY = newVelSmxY;
}

void ParticleFlyweight::SetVelocityEndMinY(float newVelSmxY)
{
	m_velocityEndMinY = newVelSmxY;
}
void ParticleFlyweight::SetVelocityEndMaxY(float newVelEmxY)
{
	m_velocityEndMaxY = newVelEmxY;
}

//********************* | GETTERS | *********************************************

float ParticleFlyweight::GetMinLife()const
{
	return m_minLife;
}

float ParticleFlyweight::GetMaxLife()const
{
	return m_maxLife;
}

float ParticleFlyweight::GetColorStartA()const
{
	return m_colorStartA;
}

float ParticleFlyweight::GetColorStartR()const
{
	return m_colorStartR;
}

float ParticleFlyweight::GetColorStartG()const
{
	return m_colorStartG;
}

float ParticleFlyweight::GetColorStartB()const
{
	return m_colorStartB;
}

float ParticleFlyweight::GetColorEndA()const
{
	return m_colorEndA;
}

float ParticleFlyweight::GetColorEndR()const
{
	return m_colorEndR;
}

float ParticleFlyweight::GetColorEndG()const
{
	return m_colorEndG;
}

float ParticleFlyweight::GetColorEndB()const
{
	return m_colorEndB;
}

float ParticleFlyweight::GetScaleStart()const
{
	return m_scaleStart;
}

float ParticleFlyweight::GetScaleEnd()const
{
	return m_scaleEnd;
}

float ParticleFlyweight::GetRotationStart()const
{
	return m_rotationStart;
}

float ParticleFlyweight::GetRotationEnd()const
{
	return m_rotationEnd;
}

float ParticleFlyweight::GetVelocityStartMinX()const
{
	return m_velocityStartMinX;
}

float ParticleFlyweight::GetVelocityStartMaxX()const
{
	return m_velocityStartMaxX;
}

float ParticleFlyweight::GetVelocityEndMinX()const
{
	return m_velocityEndMinX;
}

float ParticleFlyweight::GetVelocityEndMaxX()const
{
	return m_velocityEndMaxX;
}

float ParticleFlyweight::GetVelocityStartMinY()const
{
	return m_velocityStartMinY;
}

float ParticleFlyweight::GetVelocityStartMaxY()const
{
	return m_velocityStartMaxY;
}

float ParticleFlyweight::GetVelocityEndMinY()const
{
	return m_velocityEndMinY;
}

float ParticleFlyweight::GetVelocityEndMaxY()const
{
	return m_velocityEndMaxY;
}
