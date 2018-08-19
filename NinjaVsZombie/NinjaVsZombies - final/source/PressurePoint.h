#pragma once
#include "Unbreakable.h"
#include "TimeStamp.h"

class PressurePoint : public Unbreakable
{
	bool	m_Compressed = false;
	
	TimeStamp Compress;
	TimeStamp Decompress;
	float m_CompressedTimer = 0.0f;
	//Entity*					m_pObjToActivate;
public:
	PressurePoint(SGD::Point pos = {});
	~PressurePoint();

	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;
	virtual int				GetType(void)	const			override	{ return ENT_PRESSUREPOINT; }
	void					HandleCollision(const IEntity* pOther);

	void					SetCompressedAnim(TimeStamp com)	{ Compress = com; }
	void					SetDecompressedAnim(TimeStamp com)	{ Decompress = com; }

	bool					GetCompressed()				{ return m_Compressed; }

	void					SetCompressed(bool comp)	{ m_Compressed = comp; }
};