#include "AnimationManager.h"
#include "../TinyXML/tinyxml.h"
#include "TimeStamp.h"
#include "Game.h"

#define Alocation string("resource/Graphics/")

AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
}

AnimationManager* AnimationManager::GetInstance(void)
{
	static AnimationManager s_Instance;

	return &s_Instance;
}


void AnimationManager::Update(float dt, TimeStamp& _timestamp)
{
	LoadedAninmation[_timestamp.currAnimation].Update(dt);
}

void AnimationManager::Render(int x, int y, TimeStamp _timestamp, bool flipped, SGD::Size scale)
{
	LoadedAninmation[_timestamp.currAnimation].Render({ (float)x + Game::GetInstance()->GetWorldSpace().x, (float)y + Game::GetInstance()->GetWorldSpace().y }, flipped, scale, {});
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
		char Path[50];
		char Event[20];
		int numFrames, RectL, RectT, RectR, RectB, PosX, PosY;
		double Duration;
		int looping;
		TiXmlElement* pAnimation = root->FirstChildElement("Animation");
		for (; pAnimation;)
		{
			strcpy_s(Name, 20, pAnimation->Attribute("Name"));
			if (Name == nullptr)
				return false;

			strcpy_s(Path, 50, pAnimation->Attribute("Path"));
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
				strcpy_s(Event, 20, pFrames->Attribute("EventTrigger"));
				if (Event == nullptr)
					return false;
				new_frame.rFrame.left = (float)RectL;
				new_frame.rFrame.top = (float)RectT;
				new_frame.rFrame.right = (float)RectR + new_frame.rFrame.left;
				new_frame.rFrame.bottom = (float)RectB + new_frame.rFrame.top;
				new_frame.fDuration = (float)Duration;
				new_frame.ptAnchor.x = (float)PosX;
				new_frame.ptAnchor.y = (float)PosY;
				new_frame.eventname = Event;
				new_anim.m_vFrames.push_back(new_frame);
				pFrames = pFrames->NextSiblingElement("Frame");
			}
			//new_anim.SetPosition(SGD::Point{ (float)PosX, (float)PosY });
			new_anim.SetLooping(looping);
			new_anim.SetPlaying(1);
			new_anim.m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture((Alocation + Path).c_str());
			LoadedAninmation[Name] = new_anim;
			pAnimation = pAnimation->NextSiblingElement("Animation");
		}
	}
	return true;
}

void AnimationManager::Terminate(void)
{
	LoadedAninmation["Left"].Terminate();
	LoadedAninmation["UP"].Terminate();
	LoadedAninmation["Down"].Terminate();

	LoadedAninmation["doorcloseH"].Terminate();
	LoadedAninmation["dooropenH"].Terminate();
	LoadedAninmation["doorunlockH"].Terminate();
	LoadedAninmation["doorcloseV"].Terminate();
	LoadedAninmation["dooropenV"].Terminate();
	LoadedAninmation["doorunlockV"].Terminate();

	LoadedAninmation["PrideDeath"].Terminate();
	LoadedAninmation["PrideAttack"].Terminate();
	LoadedAninmation["PrideMove"].Terminate();
	LoadedAninmation["PrideTaunt"].Terminate();
	LoadedAninmation["PrideIdle"].Terminate();
	LoadedAninmation["TakeAKnee"].Terminate();

	LoadedAninmation["Compress"].Terminate();
	LoadedAninmation["Decompress"].Terminate();

	LoadedAninmation["ZombieWalk"].Terminate();
	LoadedAninmation["ZombieWalkUp"].Terminate();
	LoadedAninmation["ZombieWalkDown"].Terminate();
	LoadedAninmation["ZombieWalkUp"].Terminate();
	LoadedAninmation["ZombieSteal"].Terminate();
	LoadedAninmation["ZombieStealDown"].Terminate();
	LoadedAninmation["ZombieStealUp"].Terminate();
	LoadedAninmation["ZombieAttack"].Terminate();
	LoadedAninmation["ZombieAttackDown"].Terminate();
	LoadedAninmation["ZombieAttackUp"].Terminate();
	LoadedAninmation["ZombieDeath"].Terminate();

	LoadedAninmation["Walk_Left"].Terminate();
	LoadedAninmation["Whip"].Terminate();
	LoadedAninmation["LadyIdle"].Terminate();
	LoadedAninmation["LadyKiss"].Terminate();

	LoadedAninmation["GreedRun"].Terminate();
	LoadedAninmation["GreedTired"].Terminate();

}