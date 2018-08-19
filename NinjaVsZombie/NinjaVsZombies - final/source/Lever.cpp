#include "Lever.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Game.h"

Lever::Lever()
{
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/lever.png");
	m_hSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Lever.wav");
	m_hUnSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/NoLever.wav");

}

Lever::~Lever()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hSound);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hUnSound);

}

void Lever::TurnOn()
{
	m_bIsOn = true;
}

void Lever::TurnOff()
{
	
	m_bIsOn = false;
}

void Lever::Switch()
{
	m_bIsOn = !m_bIsOn;
}

void Lever::Update(float elapsedTime)
{
}

void Lever::Render(void)
{
	SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 0, 255, 0 }, {}, 0);

	if (m_bIsOn == true)
	{
		SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 0, 255, 0 }, {}, 0);
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { rect.left + GetSize().width, rect.top }, GetRotation(), GetSize(), {}, { -1.0f, 1.0f });
	}
	else
	{
		SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 0 }, {}, 0);
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { rect.left, rect.top }, GetRotation(), GetSize(), {}, { 1.0f, 1.0f });
	}
}

/*virtual*/ SGD::Rectangle Lever::GetRect(void)	const			/*override*/
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Lever::HandleCollision(const IEntity* pOther)
{

}

void Lever::PlayOnSound()
{
	SGD::AudioManager::GetInstance()->PlayAudio(m_hSound);
}
void Lever::PlayOffSound()
{
	SGD::AudioManager::GetInstance()->PlayAudio(m_hUnSound);
}