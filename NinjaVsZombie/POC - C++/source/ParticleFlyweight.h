#pragma once
class ParticleFlyweight
{
private:
	float					m_minLife;
	float					m_maxLife;
	float					m_colorStartA;
	float					m_colorStartR;
	float					m_colorStartG;
	float					m_colorStartB;
	float					m_colorEndA;
	float					m_colorEndR;
	float					m_colorEndG;
	float					m_colorEndB;
	float					m_scaleStart;
	float					m_scaleEnd;
	float					m_rotationStart;		//Rotation in radians
	float					m_rotationEnd;

	float					m_velocityStartMinX;
	float					m_velocityStartMaxX;
	float					m_velocityEndMinX;
	float					m_velocityEndMaxX;

	float					m_velocityStartMinY;
	float					m_velocityStartMaxY;
	float					m_velocityEndMinY;
	float					m_velocityEndMaxY;

public:
	ParticleFlyweight(float min = 0.0f, float max = 0.0f, float clorSA = 0.0f, float clorSR = 0.0f, float clorSG = 0.0f, float clorSB = 0.0f, float clorEA = 0.0f, float clorER = 0.0f, float clorEG = 0.0f, float clorEB = 0.0f,
		float scaleS = 0.0f, float scaleE = 0.0f, float rotaS = 0.0f, float rotaE = 0.0f, float velSMinX = 0.0f, float velSMaxX = 0.0f, float velEMinX = 0.0f, float velEMaxX = 0.0f, float velSMinY = 0.0f, float velSMaxY = 0.0f, float velEMinY = 0.0f, float velEMaxY = 0.0f);
	~ParticleFlyweight();

	//********************* | SETTERS | *********************************************

	void					SetMinLife					(float newMin);
	void					SetMaxLife					(float newMax);
	void					SetColorStartA				(float newSA);
	void					SetColorStartR				(float newSR);
	void					SetColorStartG				(float newSG);
	void					SetColorStartB				(float newSB);
	void					SetColorEndA				(float newEA);
	void					SetColorEndR				(float newER);
	void					SetColorEndG				(float newEG);
	void					SetColorEndB				(float newEB);
	void					SetScaleStart				(float newScalS);
	void					SetScaleEnd					(float newScalE);
	void					SetRotationStart			(float newRotS);
	void					SetRotationEnd				(float newRotE);

	void					SetVelocityStartMinX		(float newVelSmxX);
	void					SetVelocityStartMaxX		(float newVelSmxX);
	void					SetVelocityEndMinX			(float newVelEmnX);
	void					SetVelocityEndMaxX			(float newVelEmxX);
	void					SetVelocityStartMinY		(float newVelSmnY);
	void					SetVelocityStartMaxY		(float newVelSmxY);
	void					SetVelocityEndMinY			(float newVelEmnY);
	void					SetVelocityEndMaxY			(float newVelEmxY);

	//********************* | GETTERS | *********************************************

	float					GetMinLife					()const;
	float					GetMaxLife					()const;
	float					GetColorStartA				()const;
	float					GetColorStartR				()const;
	float					GetColorStartG				()const;
	float					GetColorStartB				()const;
	float					GetColorEndA				()const;
	float					GetColorEndR				()const;
	float					GetColorEndG				()const;
	float					GetColorEndB				()const;
	float					GetScaleStart				()const;
	float					GetScaleEnd					()const;
	float					GetRotationStart			()const;
	float					GetRotationEnd				()const;
	float					GetVelocityStartMinX		()const;
	float					GetVelocityStartMaxX		()const;
	float					GetVelocityEndMinX			()const;
	float					GetVelocityEndMaxX			()const;
	float					GetVelocityStartMinY		()const;
	float					GetVelocityStartMaxY		()const;
	float					GetVelocityEndMinY			()const;
	float					GetVelocityEndMaxY			()const;

};