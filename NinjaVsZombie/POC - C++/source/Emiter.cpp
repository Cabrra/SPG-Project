#include "Emiter.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <assert.h>
#include <wchar.h>
#include <math.h>
#include "Game.h"


Emiter::Emiter(SGD::String imagePath, SGD::Point posi, float widt, float heig, SGD::Size siz, unsigned int maxPart, unsigned int firstWav, float spawn,
	SGD::Size Imagsiz, unsigned int shape, ParticleFlyweight* fly)
{
	m_Position = posi;
	m_width = widt;
	m_height = heig;
	m_Size = siz;
	m_MaxParticles = maxPart;
	m_FirstWave = firstWav;
	m_SpawnRate = spawn;

	m_filePath = L"resource/Graphics/";
	m_filePath += imagePath;

	m_ImageSize = Imagsiz;

	SetFlyWeight(fly);

	if (shape < 0 || shape > ShapeType::COUNTER)
		m_shape = ShapeType::SHAPE_RECTANGLE;
	else
	{
		switch (shape)
		{
		case 0:
		{
				  m_shape = SHAPE_POINT;
				  break;
		}
		case 1:
		{
				  m_shape = SHAPE_LINE;
				  break;
		}
		case 2:
		{
				  m_shape = SHAPE_RECTANGLE;
				  break;
		}
		case 3:
		{
				  m_shape = SHAPE_CIRCLE;
				  break;
		}
		}
	}

	// test



	//m_ParticleFlyweight->SetColorStartA(fly->GetColorStartA());
	//m_ParticleFlyweight->SetColorStartR(fly->GetColorStartR());
	//m_ParticleFlyweight->SetColorStartG(fly->GetColorStartG());
	//m_ParticleFlyweight->SetColorStartB(fly->GetColorStartB());
	//m_ParticleFlyweight->SetColorEndA(fly->GetColorEndA());
	//m_ParticleFlyweight->SetColorEndR(fly->GetColorEndR());
	//m_ParticleFlyweight->SetColorEndG(fly->GetColorEndG());
	//m_ParticleFlyweight->SetColorEndB(fly->GetColorEndB());
	//m_ParticleFlyweight->SetMaxLife(fly->GetMaxLife());
	//m_ParticleFlyweight->SetMinLife(fly->GetMinLife());
	//m_ParticleFlyweight->SetRotationStart(fly->Get);
	//m_ParticleFlyweight->SetRotationEnd(fly->Get);
	//m_ParticleFlyweight->SetScaleStart(fly->Get);
	//m_ParticleFlyweight->SetScaleEnd(fly->Get);
	//
	////velocity
	//m_ParticleFlyweight->SetVelocityStartMinX(-37.05f);
	//m_ParticleFlyweight->SetVelocityStartMaxX(-10.8f);
	//m_ParticleFlyweight->SetVelocityEndMinX(15.0f);
	//m_ParticleFlyweight->SetVelocityEndMaxX(30.0f);

	//m_ParticleFlyweight->SetVelocityStartMinY(12.0f);
	//m_ParticleFlyweight->SetVelocityStartMaxY(22.0f);
	//m_ParticleFlyweight->SetVelocityEndMinY(-38.0f);
	//m_ParticleFlyweight->SetVelocityEndMaxY(-13.0f);

	InitializeEmiter();
}

Emiter::~Emiter()
{
	assert(m_unRefCount == 0 && "~Emiter - emiter was destroyed while references remain!");
	delete m_ParticleFlyweight;

	for (unsigned int i = 0; i < m_LiveList.size();)
	{
		delete m_LiveList.front();
		m_LiveList.erase(m_LiveList.begin());
	}

	for (unsigned int i = 0; i < m_DeadList.size();)
	{
		delete m_DeadList.front();
		m_DeadList.erase(m_DeadList.begin());
	}

	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hParticle);
}

void Emiter::InitializeEmiter()
{

	for (unsigned int i = 0; i < m_MaxParticles; i++)
	{
		Particle* newPart = new Particle();
		m_DeadList.push_back(newPart);
	}

	if (m_FirstWave > 0 && m_FirstWave < m_FirstWave)
	{
		for (unsigned int i = 0; i < m_FirstWave; i++)
			SpawnParticle();
	}

	elapsedSpawn = 0;

	m_hParticle = SGD::GraphicsManager::GetInstance()->LoadTexture(m_filePath.c_str());
}

void Emiter::Update(float dt)
{
	elapsedSpawn += dt;

	SGD::Rectangle myRect = SGD::Rectangle(m_Position - m_Size, m_Position + m_Size);

	std::list<Particle*> iterator(m_LiveList.begin(), m_LiveList.end());
	for each (Particle* particle in iterator)
	{
		SGD::Size size = { particle->GetScale(), particle->GetScale() };
		//	SGD::Rectangle partRect = SGD::Rectangle(particle->GetPosition() - size / 2, particle->GetPosition() + size / 2);

		if (particle->GetLife() < 0) //|| (m_isOuter == false && !myRect.IsIntersecting(partRect)) ) //check range
		{
			m_LiveList.remove(particle);
			m_DeadList.push_back(particle);
		}
		else
		{
			particle->SetLife(particle->GetLife() - dt);
			//color
			particle->SetColorA(LerpFunction(m_ParticleFlyweight->GetColorStartA(), m_ParticleFlyweight->GetColorEndA(), 1 - (particle->GetLife() / particle->GetOriginal())));
			particle->SetColorR(LerpFunction(m_ParticleFlyweight->GetColorStartR(), m_ParticleFlyweight->GetColorEndR(), 1 - (particle->GetLife() / particle->GetOriginal())));
			particle->SetColorG(LerpFunction(m_ParticleFlyweight->GetColorStartG(), m_ParticleFlyweight->GetColorEndG(), 1 - (particle->GetLife() / particle->GetOriginal())));
			particle->SetColorB(LerpFunction(m_ParticleFlyweight->GetColorStartB(), m_ParticleFlyweight->GetColorEndB(), 1 - (particle->GetLife() / particle->GetOriginal())));

			particle->SetScale(LerpFunction(m_ParticleFlyweight->GetScaleStart(), m_ParticleFlyweight->GetScaleEnd(), 1 - (particle->GetLife() / particle->GetOriginal())));
			particle->SetRotation(LerpFunction(m_ParticleFlyweight->GetRotationStart(), m_ParticleFlyweight->GetRotationEnd(), 1 - (particle->GetLife() / particle->GetOriginal())));

			particle->SetVelocityX(particle->GetVelocityX() + particle->GetchangeVelocityX() * dt);
			particle->SetVelocityY(particle->GetVelocityY() + particle->GetchangeVelocityY() * dt);
			//particle->SetSize(m_ImageSize * particle->GetScale());

			SGD::Point posChange;
			posChange.x = particle->GetPosition().x + particle->GetVelocityX() * dt;
			posChange.y = particle->GetPosition().y + particle->GetVelocityY() * dt;

			particle->SetPosition(posChange);
		}
	}

if (elapsedSpawn > m_SpawnRate && m_DeadList.size() != 0)
{
	if (m_SpawnRate > 0)
	{
		for (unsigned int i = 0; i < m_FirstWave; i++)
		{
			if (m_DeadList.size() > 0)
				SpawnParticle();
			else
				break;
		}
	}
	else if (m_DeadList.size() > 0)
		SpawnParticle();


	elapsedSpawn = 0.0f;

}
}

void Emiter::SpawnParticle()
{
	Particle* part = m_DeadList.front();
	float current = m_ParticleFlyweight->GetMinLife() + rand() % (int)(m_ParticleFlyweight->GetMaxLife() - m_ParticleFlyweight->GetMinLife());
	part->SetLife(current);
	part->SetOriginal(current);

	float posX;
	float posY;

	switch (m_shape)
	{
	case(SHAPE_LINE) : // line
	{
						   SGD::Point one = SGD::Point();
						   SGD::Point two = SGD::Point();
						   SGD::Point three = SGD::Point();

						   float slopeX;
						   float slopeY;

						   if (m_Position.x == m_width)
						   {
							   slopeX = 0.0f;
							   slopeY = 1.0f;
						   }
						   else if (m_Position.y == m_height)
						   {
							   slopeY = 0.0f;
							   slopeX = 1.0f;
						   }
						   else
							   slopeX = slopeY = abs((float)((m_Position.y - m_height) / (m_Position.x - m_width)));

						   float length;
						   length = sqrt(((m_Position.x - m_width) * (m_Position.x - m_width)) + ((m_Position.y - m_height) * (m_Position.y - m_height)));


						   if (slopeX == 0 || slopeY == 0)
						   {
							   posX = (m_Position.x - ((length / 2) * slopeX)) + rand() % (int)(m_Position.x + ((length / 2) * slopeX) - (m_Position.x - ((length / 2) * slopeX) + 1));
							   posY = (m_Position.y - ((length / 2) * slopeY)) + rand() % (int)(m_Position.y + ((length / 2) * slopeY) - (m_Position.y - ((length / 2) * slopeY) + 1));

						   }
						   else if (slopeX != 0 && slopeY != 0)
						   {
							   do
							   {
								   posX = (m_Position.x - ((length / 2) * slopeX)) + rand() % (int)(((m_Position.x + ((length / 2) * slopeX)) + m_Position.x));
								   posY = (m_Position.y - ((length / 2) * slopeY)) + rand() % (int)(((m_Position.y + ((length / 2) * slopeY)) + m_Position.y));

								   one.x = m_Position.x;
								   one.y = m_Position.y;

								   two.x = m_width;
								   two.y = m_height;

								   three.x = posX;
								   three.y = posY;

							   } while (!IsOnLine(one, two, three));
						   }


						   break;
	}
	case(SHAPE_RECTANGLE) : //Rectangle
	{
								posX = (m_Position.x - m_width) + rand() % ((int)((m_Position.x + m_width) - (m_Position.x - m_width)));
								posY = (m_Position.y - m_height) + rand() % ((int)((m_Position.y + m_height) - (m_Position.y - m_height)));

								break;
	}
	case(SHAPE_CIRCLE) : //Circle
	{
							 float distance;
							 do
							 {
								 posX = (m_Position.x - m_width) + rand() % ((int)((m_Position.x + m_width) - (m_Position.x - m_width)));
								 posY = (m_Position.y - m_width) + rand() % ((int)((m_Position.y + m_width) - (m_Position.y - m_width)));

								 distance = sqrt((pow(posX - m_Position.x, 2.0f) + pow(posY - m_Position.y, 2.0f)));

								 //(x - center_x)^2 + (y - center_y)^2 < radius^2.
							 } while (distance > m_width);
							 break;
	}
	default:
	{
			   posX = m_Position.x;
			   posY = m_Position.y;
			   break;
	}
	}


	part->SetPosition({ posX, posY });
	part->SetColorA(m_ParticleFlyweight->GetColorStartA());
	part->SetColorR(m_ParticleFlyweight->GetColorStartR());
	part->SetColorG(m_ParticleFlyweight->GetColorStartG());
	part->SetColorB(m_ParticleFlyweight->GetColorStartB());
	part->SetScale(m_ParticleFlyweight->GetScaleStart());
	part->SetRotation(m_ParticleFlyweight->GetRotationStart());

	float velX;
	if (m_ParticleFlyweight->GetVelocityStartMinX() == 0 || m_ParticleFlyweight->GetVelocityStartMaxX() == 0)
		velX = 0.0f;
	else
		velX = m_ParticleFlyweight->GetVelocityStartMinX() + (float)(rand() % (int)(m_ParticleFlyweight->GetVelocityStartMaxX() - m_ParticleFlyweight->GetVelocityStartMinX()) + rand() / (RAND_MAX + 1));

	part->SetVelocityX(velX);

	float velY;

	if (m_ParticleFlyweight->GetVelocityStartMinY() == 0 || m_ParticleFlyweight->GetVelocityStartMaxY() == 0)
		velY = 0.0;
	else
		velY = m_ParticleFlyweight->GetVelocityStartMinY() + (float)(rand() % (int)(m_ParticleFlyweight->GetVelocityStartMaxY() - m_ParticleFlyweight->GetVelocityStartMinY()) + rand() / (RAND_MAX + 1));

	part->SetVelocityY(velY);

	//Calculate changes
	ChangeRatioCalculator(part);

	m_DeadList.pop_front();
	m_LiveList.push_back(part);
}

void Emiter::Render()
{
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle{ m_Position - m_Size, m_Position + m_Size }, { 255, 255, 0 });

	std::list<Particle*> iterator(m_LiveList.begin(), m_LiveList.end());
	for each (Particle* particle in iterator)
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hParticle,
		{ (particle->GetPosition().x - (m_ImageSize.width / 2) * particle->GetScale()) + Game::GetInstance()->GetWorldSpace().x,
		(particle->GetPosition().y - (m_ImageSize.height / 2) * particle->GetScale()) + Game::GetInstance()->GetWorldSpace().y
		},
		particle->GetRotation(), (m_ImageSize / 2),
		SGD::Color{ (unsigned char)particle->GetColorA(), (unsigned char)particle->GetColorR(),
		(unsigned char)particle->GetColorG(), (unsigned char)particle->GetColorB() }, // colors
		{ particle->GetScale(), particle->GetScale() }
		);

		//		SGD::GraphicsManager::GetInstance()->DrawLine(particle->GetPosition(), particle->GetPosition() - particle->GetSize() / 2);
	}
}

void Emiter::ChangeRatioCalculator(Particle* parti)
{

	float changeVelX;
	float changeVelY;

	float velX;

	if (m_ParticleFlyweight->GetVelocityEndMinX() == 0 || m_ParticleFlyweight->GetVelocityEndMaxX() == 0)
		velX = 0;
	else
		velX = m_ParticleFlyweight->GetVelocityEndMinX() + (float)(rand() % (int)(m_ParticleFlyweight->GetVelocityEndMaxX() - m_ParticleFlyweight->GetVelocityEndMinX()) + rand() / (RAND_MAX + 1));

	changeVelX = (velX / parti->GetLife());

	float velY;
	if (m_ParticleFlyweight->GetVelocityEndMinY() == 0 || m_ParticleFlyweight->GetVelocityEndMaxY() == 0)
		velY = 0;
	else
		velY = m_ParticleFlyweight->GetVelocityEndMinY() + (float)(rand() % (int)(m_ParticleFlyweight->GetVelocityEndMaxY() - m_ParticleFlyweight->GetVelocityEndMinY()) + rand() / (RAND_MAX + 1));
	changeVelY = (velY / parti->GetLife());

	parti->SetchangeVelocityX(changeVelX);
	parti->SetchangeVelocityY(changeVelY);

}

void Emiter::AddRef(void)
{
	assert(m_unRefCount != 0xFFFFFFFF && "Entity::AddRef - maximum reference count has been exceeded");

	++m_unRefCount;
}

void Emiter::Release(void)
{
	--m_unRefCount;

	if (m_unRefCount == 0)
		delete this;
}

//********************* | SETTERS | *********************************************

void Emiter::SetPosition(SGD::Point newPos)
{
	m_Position = newPos;
}

//void Emiter::SetSpreadX(float newSpreX)
//{
//	m_SpreadX = newSpreX;
//}
//
//void Emiter::SetSpreadY(float newSpreY)
//{
//	m_SpreadY = newSpreY;
//}

void Emiter::SetSize(SGD::Size newSiz)
{
	m_Size = newSiz;
}

void Emiter::SetMaxParticles(unsigned int newMax)
{
	m_MaxParticles = newMax;
}

void Emiter::SetFirstWave(unsigned int newFirst)
{
	m_FirstWave = newFirst;
}

void Emiter::SetSpawnRate(float newSpawnRate)
{
	m_SpawnRate = newSpawnRate;
}

void Emiter::SetFlyWeight(ParticleFlyweight* fweight)
{
	assert(fweight != nullptr && "Emiter::SetFlyWeight - flyweight was NULL!");

	m_ParticleFlyweight = new ParticleFlyweight;

	m_ParticleFlyweight->SetColorStartA(fweight->GetColorStartA());
	m_ParticleFlyweight->SetColorStartR(fweight->GetColorStartR());
	m_ParticleFlyweight->SetColorStartG(fweight->GetColorStartG());
	m_ParticleFlyweight->SetColorStartB(fweight->GetColorStartB());
	m_ParticleFlyweight->SetColorEndA(fweight->GetColorEndA());
	m_ParticleFlyweight->SetColorEndR(fweight->GetColorEndR());
	m_ParticleFlyweight->SetColorEndG(fweight->GetColorEndG());
	m_ParticleFlyweight->SetColorEndB(fweight->GetColorEndB());
	m_ParticleFlyweight->SetMaxLife(fweight->GetMaxLife());
	m_ParticleFlyweight->SetMinLife(fweight->GetMinLife());
	m_ParticleFlyweight->SetRotationStart(fweight->GetRotationStart());
	m_ParticleFlyweight->SetRotationEnd(fweight->GetRotationEnd());
	m_ParticleFlyweight->SetScaleStart(fweight->GetScaleStart());
	m_ParticleFlyweight->SetScaleEnd(fweight->GetScaleEnd());
	//velocity 

	m_ParticleFlyweight->SetVelocityStartMinX(fweight->GetVelocityStartMinX());
	m_ParticleFlyweight->SetVelocityStartMaxX(fweight->GetVelocityStartMaxX());
	m_ParticleFlyweight->SetVelocityEndMinX(fweight->GetVelocityEndMinX());
	m_ParticleFlyweight->SetVelocityEndMaxX(fweight->GetVelocityEndMaxX());
	m_ParticleFlyweight->SetVelocityStartMinY(fweight->GetVelocityStartMinY());
	m_ParticleFlyweight->SetVelocityStartMaxY(fweight->GetVelocityStartMaxY());
	m_ParticleFlyweight->SetVelocityEndMinY(fweight->GetVelocityEndMinY());
	m_ParticleFlyweight->SetVelocityEndMaxY(fweight->GetVelocityEndMaxY());

}

//********************* | GETTERS | *********************************************


SGD::Point		Emiter::GetPosition()const
{
	return m_Position;
}

//float Emiter::GetSpreadX()const
//{
//	return m_SpreadX;
//}
//
//float Emiter::GetSpreadY()const
//{
//	return m_SpreadY;
//}

SGD::Size		Emiter::GetSize()const
{
	return m_Size;
}

unsigned int	Emiter::GetMaxParticles()const
{
	return m_MaxParticles;
}

unsigned int Emiter::GetFirstWave()const
{
	return m_FirstWave;
}

float Emiter::GetSpawnRate()const
{
	return m_SpawnRate;
}

bool Emiter::IsOnLine(SGD::Point startPoint, SGD::Point endPoint, SGD::Point checkPoint)
{
	float c1;
	float c2;

	if ((endPoint.y - startPoint.y) == 0.0f)
		c1 = 1;
	else
		c1 = (endPoint.y - startPoint.y);

	if ((endPoint.x - startPoint.x) == 0.0f)
		c2 = 1;
	else
		c2 = (endPoint.x - startPoint.x);

	return ((checkPoint.y - startPoint.y) / c1)
		== ((checkPoint.x - startPoint.x) / c2);
}

float Emiter::LerpFunction(float start, float end, float ratio)
{
	float aux = (end - start)* ratio + start;
	return aux;
}