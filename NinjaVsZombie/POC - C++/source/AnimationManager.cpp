#include "AnimationManager.h"
#include "../TinyXML/tinyxml.h"
#include "TimeStamp.h"
#include "Game.h"

AnimationManager::AnimationManager()
{
	testanimation.Initialize();
	testanimation2.SetsetAnim(2);
	LoadedAninmation["test"] = testanimation;
	testanimation2.Initialize();
	LoadedAninmation["test2"] = testanimation2;

}

AnimationManager::~AnimationManager()
{
}

#if 0
void AnimationManager::AddEntity(AnchorPointAnimation* pEntity, unsigned int bucket)
{
	// Validate the parameter
	assert(pEntity != nullptr && "AnimationManager::AddEntity - parameter cannot be null");

	// Expand the table?
	if (bucket >= m_tEntities.size())
		m_tEntities.resize(bucket + 1);


	// Append the entity into the specified vector
	m_tEntities[bucket].push_back(pEntity);

	// Hold a reference to keep the entity in memory
	pEntity->AddRef();
}
void AnimationManager::RemoveEntity(AnchorPointAnimation* pEntity, unsigned int bucket)
{

	// Validate the iteration state
	assert(m_bIterating == false && "AnimationManager::RemoveAll - cannot remove while iterating");

	// Validate the parameters
	assert(pEntity != nullptr && "AnimationManager::RemoveEntity - cannot remove NULL");
	assert(bucket < m_tEntities.size() && "AnimationManager::RemoveEntity - invalid bucket");


	// Try to find the entity
	EntityVector& vec = m_tEntities[bucket];
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == pEntity)
		{
			// Remove the entity
			vec.erase(vec.begin() + i);
			pEntity->Release();
			break;
		}
	}
}
void AnimationManager::RemoveEntity(AnchorPointAnimation* pEntity)
{
	// Validate the iteration state
	assert(m_bIterating == false && "AnimationManager::RemoveAll - cannot remove while iterating");

	// Validate the parameters
	assert(pEntity != nullptr && "AnimationManager::RemoveEntity - pointer cannot be null");


	// Try to find the entity in any buckect
	for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++)
	{
		EntityVector& vec = m_tEntities[bucket];
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			if (vec[i] == pEntity)
			{
				// Remove the entity
				vec.erase(vec.begin() + i);
				pEntity->Release();
				return;
			}
		}
	}
}
void AnimationManager::RemoveAll(unsigned int bucket)
{
	// Validate the iteration state
	assert(m_bIterating == false && "AnimationManager::RemoveAll - cannot remove while iterating");

	// Validate the parameter
	assert(bucket < m_tEntities.size() && "AnimationManager::RemoveAll - invalid bucket");


	// Lock the iterator
	m_bIterating = true;
	{
		// Release the reference to EVERY entity
		EntityVector& vec = m_tEntities[bucket];
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			vec[i]->Release();
			vec[i] = nullptr;
		}

		vec.clear();
	}
	// Unlock the iterator
	m_bIterating = false;
}
void AnimationManager::RemoveAll(void)
{
	// Validate the iteration state
	assert(m_bIterating == false && "AnimationManager::RemoveAll - cannot remove while iterating");

	// Lock the iterator
	m_bIterating = true;
	{
		// Release every entity
		for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++)
		{
			EntityVector& vec = m_tEntities[bucket];
			for (unsigned int i = 0; i < vec.size(); i++)
			{

				vec[i]->Release();
				vec[i] = nullptr;
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;


	// Collapse the table
	m_tEntities.clear();
}

void AnimationManager::UpdateAll(float elapsedTime)
{
	// Validate the iteration state
	assert(m_bIterating == false && "AnimationManager::UpdateAll - cannot update while iterating");

	// Lock the iterator
	m_bIterating = true;
	{
		// Update every entity
		for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++)
		{
			EntityVector& vec = m_tEntities[bucket];
			for (unsigned int i = 0; i < vec.size(); i++)
				vec[i]->Update(elapsedTime);
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}
void AnimationManager::RenderAll(void)
{
	// Validate the iteration state
	assert(m_bIterating == false && "AnimationManager::RenderAll - cannot render while iterating");

	// Lock the iterator
	m_bIterating = true;
	{
		// Render every entity
		for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++)
		{
			EntityVector& vec = m_tEntities[bucket];
			for (unsigned int i = 0; i < vec.size(); i++)
			{
				vec[i]->Render(vec[i]->GetPosition(), false, vec[i]->GetScale(), { 255, 255, 255 });

			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}

/**************************************************************/
// CheckCollisions
//	- check collision between the entities within the two buckets
void AnimationManager::CheckCollisions(unsigned int bucket1, unsigned int bucket2)
{
#if 0
	// Validate the iteration state
	assert(m_bIterating == false && "AnimationManger::CheckCollisions - cannot collide while iterating");

	// Quietly validate the parameters
	if (bucket1 >= m_tEntities.size()
		|| bucket2 >= m_tEntities.size()
		|| m_tEntities[bucket1].size() == 0
		|| m_tEntities[bucket2].size() == 0)
		return;


	// Lock the iterator
	m_bIterating = true;
	{
		// Are they different buckets?
		if (bucket1 != bucket2)
		{
			// Which bucket is smaller?
			//	should be the outer loop for less checks (n0)*(n1+1) + 1
			EntityVector* pVec1 = &m_tEntities[bucket1];
			EntityVector* pVec2 = &m_tEntities[bucket2];

			if (pVec2->size() < pVec1->size())
			{
				EntityVector* pTemp = pVec1;
				pVec1 = pVec2;
				pVec2 = pTemp;
			}

			EntityVector& vec1 = *pVec1;
			EntityVector& vec2 = *pVec2;


			// Iterate through the smaller bucket
			for (unsigned int i = 0; i < vec1.size(); i++)
			{
				// Iterate through the larger bucket
				for (unsigned int j = 0; j < vec2.size(); j++)
				{
					// Ignore self-collision and not dead
					if (vec1[i] == vec2[j])
						continue;

					// Local variables help with debugging
					SGD::Rectangle rEntity1 = vec1[i]->GetRect();
					SGD::Rectangle rEntity2 = vec2[j]->GetRect();

					// Check for collision between the entities
					if (rEntity1.IsIntersecting(rEntity2) == true)
					{
						// Both objects handle collision
						vec1[i]->HandleCollision(vec2[j]);
						vec2[j]->HandleCollision(vec1[i]);
					}
				}
			}
		}
		else // bucket1 == bucket2
		{
			EntityVector& vec = m_tEntities[bucket1];

			// Optimized loop to ensure objects do not collide with
			// each other twice
			for (unsigned int i = 0; i < vec.size() - 1; i++)
			{
				for (unsigned int j = i + 1; j < vec.size(); j++)
				{
					// Ignore self-collision
					if (vec[i] == vec[j])
						continue;

					// Local variables help with debugging
					SGD::Rectangle rEntity1 = vec[i]->GetRect();
					SGD::Rectangle rEntity2 = vec[j]->GetRect();

					// Check for collision between the entities
					if (rEntity1.IsIntersecting(rEntity2) == true)
					{
						// Both objects handle collision
						vec[i]->HandleCollision(vec[j]);
						vec[j]->HandleCollision(vec[i]);
					}
				}
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
#endif // 0

}

#endif // 0

AnimationManager* AnimationManager::GetInstance(void)
{
	static AnimationManager s_Instance;

	return &s_Instance;
}


void AnimationManager::Update(float dt, TimeStamp& _timestamp)
{
	LoadedAninmation[_timestamp.currAnimation].Update(dt);
	_timestamp.currFrame++;
	if (_timestamp.currAnimation == "test2")
	{
		if (_timestamp.currFrame >= 100)
		{
			LoadedAninmation[_timestamp.currAnimation].SetPlaying(1);
		}
		else
		{
			LoadedAninmation[_timestamp.currAnimation].SetPlaying(0);
		}
	}
}

void AnimationManager::Render(int x, int y, TimeStamp _timestamp)
{
	LoadedAninmation[_timestamp.currAnimation].Render({ (float)x + Game::GetInstance()->GetWorldSpace().x, (float)y + Game::GetInstance()->GetWorldSpace().y }, false, 1, {});
}

void  AnimationManager::SaveXML()
{
	//create tinyXML Document
	TiXmlDocument doc;

	//create a tinyXML Declaration
	TiXmlDeclaration* pDecl = new TiXmlDeclaration{ "1.0", "utf-8", "" };

	//link the declaration to the doc
	doc.LinkEndChild(pDecl);

	//all the levels
	TiXmlElement* pRoot = new TiXmlElement{ "Animation_Info" };

	//link the root to the document
	doc.LinkEndChild(pRoot);

	TiXmlElement* pAnimation = new TiXmlElement{ "Animation" };
	pRoot->LinkEndChild(pAnimation);

	pAnimation->SetAttribute("Name", "Test Animation");
	pAnimation->SetAttribute("Path", "resource/Graphics/Player.png");
	pAnimation->SetAttribute("NumofFrames", 10);
	for (unsigned int i = 0; i < 10; i++)
	{
		TiXmlElement* pFrame = new TiXmlElement{ "Frame" };
		pFrame->SetAttribute("SourceRecL", -20);
		pFrame->SetAttribute("SourceRecT", 10);
		pFrame->SetAttribute("SourceRecR", 60);
		pFrame->SetAttribute("SourceRecB", 72);
		pFrame->SetAttribute("PositionX", 118);
		pFrame->SetAttribute("PositionY", 233);
		pFrame->SetDoubleAttribute("Duration", 0.1);
		pFrame->SetAttribute("Looping", 1);
		pFrame->SetAttribute("Playing", 1);
		pFrame->SetAttribute("Finished", 0);
		pAnimation->LinkEndChild(pFrame);
	}


	doc.SaveFile("resource/XML/testfileAnimation.xml");

}

bool AnimationManager::LoadXML(const char* file)
{
	TiXmlDocument doc;

	if (doc.LoadFile(file) == false)
		return false;
	else
	{
		TiXmlElement* root = doc.RootElement();

		if (root == nullptr)
			return false;
		int numAnimation;
		root->Attribute("NumberofAnimations", &numAnimation);
		//load map/level info
			char Name[20];
			char Path[102];
			int numFrames, RectL, RectT, RectR, RectB, PosX, PosY;
			double Duration;
			int looping;
			TiXmlElement* pAnimation = root->FirstChildElement("Animation");
		for (;pAnimation;)
		{


			strcpy_s(Name, 20, pAnimation->Attribute("Name"));
			if (Name == nullptr)
				return false;

			strcpy_s(Path, 102, pAnimation->Attribute("Path"));

			if (Path == nullptr)
				return false;

			pAnimation->Attribute("Looping", &looping);
			AnchorPointAnimation new_anim;

			TiXmlElement* pFrames = pAnimation->FirstChildElement("Frame");
			pAnimation->Attribute("NumofFrames", &numFrames);
			for (; pFrames;)
			{

				AnchorPointAnimation::Frame new_frame;
				pFrames->Attribute("FrameRectX", &RectL);
				pFrames->Attribute("FrameRectY", &RectT);
				pFrames->Attribute("FrameRectWidth", &RectR);
				pFrames->Attribute("FrameRectHeight", &RectB);
				pFrames->Attribute("AnchorpointX", &PosX);
				pFrames->Attribute("AnchorpointY", &PosY);
				pFrames->Attribute("Duration", &Duration);
				new_frame.rFrame.left = (float)RectL;
				new_frame.rFrame.top = (float)RectT;
				new_frame.rFrame.right = (float)RectR;
				new_frame.rFrame.bottom = (float)RectB;
				new_frame.fDuration = (float)Duration;
				new_anim.m_vFrames.push_back(new_frame);
				pFrames = pFrames->NextSiblingElement("Frame");

			}
			new_anim.SetPosition(SGD::Point{ (float)PosX, (float)PosY });
			new_anim.SetLooping(looping);
			new_anim.m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(Path);
			LoadedAninmation[Name] = new_anim;
			pAnimation = pAnimation->NextSiblingElement("Animation");
		}
	}
	return true;
}

void AnimationManager::Terminate(void)
{
	testanimation.Terminate();
	testanimation2.Terminate();
	LoadedAninmation["Test Animation"].Terminate();
}


//void AnimationManager::Update(float dt)
//{
//	vecAnimations[animations].Update(dt);
//}
//
//void AnimationManager::Render(SGD::Point position, bool flipped,
//	float scale, SGD::Color color)
//{
//	vecAnimations[animations].Render(position, flipped, scale, color);
//}
//
//void AnimationManager::SetImage()
//{
//	if (animations == 0)
//	{
//		m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
//			L"SGD_Anim_Explosion.png");
//	}
//	if (animations == 1)
//	{
//		m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
//			L"SGD_Anim_Explosion.png");
//	}
//	if (animations == 2)
//	{
//
//	}
//}