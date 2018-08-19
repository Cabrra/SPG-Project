#include "BossKey.h"
#include "DestroyEntityMessage.h"
#include "Player.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "GamePlayState.h"

BossKey::BossKey()
{
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/oot_boss_key_by_blueamnesiac-d6scrg8.png");
	m_hSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/soundeffects/pizza take.wav");

	//SetPosition({ 600, 800 });
	SetSize({ 32, 32});

}

BossKey::~BossKey()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hSound);
}

/*virtual*/ void			BossKey::Update(float elapsedTime)		/*override*/
{

}
/*virtual*/ void			BossKey::Render(void)					/*override*/
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, GetPositionInWorldSpace(), GetRotation(), GetSize() / 2, {}, { 0.1f, 0.1f });
}
/*virtual*/ void			BossKey::HandleCollision(const IEntity* pOther)	/*override*/
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		GamePlayState::GetInstance()->CreatMessage(GamePlayState::GetInstance()->_BossKey);

		Player* mypla = (Player*)pOther;
		mypla->SetHasKey(true);
		SGD::AudioManager::GetInstance()->PlayAudio(m_hSound);
		//destroy this
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}
