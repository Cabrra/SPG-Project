#include "ParticleManager.h"

#include <cassert>
#include "Emiter.h"
#include "ParticleFlyweight.h"
#include "../TinyXML/tinyxml.h"
#include <string>

/**************************************************************/
void ParticleManager::AddEmiter(Emiter* pEmiter, unsigned int bucket)
{
	// Validate the parameter
	assert(pEmiter != nullptr && "ParticleManager::AddEmiter - parameter cannot be null");


	// Expand the table?
	if (bucket >= m_tEmiters.size())
		m_tEmiters.resize(bucket + 1);


	// Append the Emiter into the specified vector
	m_tEmiters[bucket].push_back(pEmiter);

	// Hold a reference to keep the Emiter in memory
	pEmiter->AddRef();
}


/**************************************************************/
// RemoveEmiter
//	- remove the Emiter from the specified bucket
//	- release the reference to the Emiter
void ParticleManager::RemoveEmiter(Emiter* pEmiter, unsigned int bucket)
{
	// Validate the iteration state
	assert(m_bIterating == false && "ParticleManager::RemoveEmiter - cannot remove while iterating");

	// Validate the parameters
	assert(pEmiter != nullptr && "ParticleManager::RemoveEmiter - cannot remove NULL");
	assert(bucket < m_tEmiters.size() && "ParticleManager::RemoveEmiter - invalid bucket");


	// Try to find the Emiter
	EmiterVector& vec = m_tEmiters[bucket];
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == pEmiter)
		{
			// Remove the Emiter
			vec.erase(vec.begin() + i);
			pEmiter->Release();
			break;
		}
	}
}


/**************************************************************/
// RemoveEmiter
//	- remove & release the Emiter from any bucket
void ParticleManager::RemoveEmiter(Emiter* pEmiter)
{
	// Validate the iteration state
	assert(m_bIterating == false && "ParticleManager::RemoveEmiter - cannot remove while iterating");

	// Validate the parameters
	assert(pEmiter != nullptr && "ParticleManager::RemoveEmiter - pointer cannot be null");


	// Try to find the Emiter in any buckect
	for (unsigned int bucket = 0; bucket < m_tEmiters.size(); bucket++)
	{
		EmiterVector& vec = m_tEmiters[bucket];
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			if (vec[i] == pEmiter)
			{
				// Remove the Emiter
				vec.erase(vec.begin() + i);
				pEmiter->Release();
				return;
			}
		}
	}
}


/**************************************************************/
// RemoveAll
//	- remove all entities from a specific bucket
void ParticleManager::RemoveAll(unsigned int unBucket)
{
	// Validate the iteration state
	assert(m_bIterating == false && "ParticleManager::RemoveAll - cannot remove while iterating");

	// Validate the parameter
	assert(unBucket < m_tEmiters.size() && "ParticleManager::RemoveAll - invalid bucket");


	// Lock the iterator
	m_bIterating = true;
	{
		// Release the reference to EVERY Emiter
		EmiterVector& vec = m_tEmiters[unBucket];
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



/**************************************************************/
// RemoveAll
//	- release each Emiter in the table
void ParticleManager::RemoveAll(void)
{
	// Validate the iteration state
	assert(m_bIterating == false && "ParticleManager::RemoveAll - cannot remove while iterating");

	// Lock the iterator
	m_bIterating = true;
	{
		// Release every Emiter
		for (unsigned int bucket = 0; bucket < m_tEmiters.size(); bucket++)
		{
			EmiterVector& vec = m_tEmiters[bucket];
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
	m_tEmiters.clear();
}


/**************************************************************/
// UpdateAll
//	- update each Emiter in the table
void ParticleManager::UpdateAll(float elapsedTime)
{
	// Validate the iteration state
	assert(m_bIterating == false && "ParticleManager::UpdateAll - cannot update while iterating");

	// Lock the iterator
	m_bIterating = true;
	{
		// Update every Emiter
		for (unsigned int bucket = 0; bucket < m_tEmiters.size(); bucket++)
		{
			EmiterVector& vec = m_tEmiters[bucket];
			for (unsigned int i = 0; i < vec.size(); i++)
				vec[i]->Update(elapsedTime);
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}


/**************************************************************/
// RenderAll
//	- render each Emiter in the table
void ParticleManager::RenderAll(void)
{
	// Validate the iteration state
	assert(m_bIterating == false && "ParticleManager::RenderAll - cannot render while iterating");

	// Lock the iterator
	m_bIterating = true;
	{
		// Render every Emiter
		for (unsigned int bucket = 0; bucket < m_tEmiters.size(); bucket++)
		{
			EmiterVector& vec = m_tEmiters[bucket];
			for (unsigned int i = 0; i < vec.size(); i++)
				vec[i]->Render();
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}

bool ParticleManager::LoadEmiter(const char* file)
{
	TiXmlDocument doc;

	if (doc.LoadFile(file) == false)
		return false;
	else
	{
		TiXmlElement* root = doc.RootElement();

		if (root == nullptr)
			return false;

		TiXmlElement* startPoint = root->FirstChildElement("Emiter");

		if (startPoint == nullptr)
			return false;

		//load map/level info
		TiXmlElement* emiterInfo = startPoint->FirstChildElement("Path");

		if (emiterInfo == nullptr)
			return false;

		char imagePath[24];

		strcpy_s(imagePath, 24, emiterInfo->Attribute("Value"));

		if (imagePath == nullptr)
			return false;

		emiterInfo = startPoint->FirstChildElement("Width");
		int width;
		emiterInfo->Attribute("Value", &width);

		if (width < 0)
			return false;

		emiterInfo = startPoint->FirstChildElement("Height");
		int height;
		emiterInfo->Attribute("Value", &height);

		if (height < 0)
			return false;
		
		emiterInfo = startPoint->FirstChildElement("PositionX");
		int positionx;
		emiterInfo->Attribute("Value", &positionx);

		emiterInfo = startPoint->FirstChildElement("PositionY");
		int positiony;
		emiterInfo->Attribute("Value", &positiony);

		//emiterInfo = startPoint->FirstChildElement("SpreadX");
		//int spreadX;
		//emiterInfo->Attribute("Value", &spreadX);

		//emiterInfo = startPoint->FirstChildElement("SpreadY");
		//int spreadY;
		//emiterInfo->Attribute("Value", &spreadY);

		emiterInfo = startPoint->FirstChildElement("ImageSizeWidh");
		int imageSizW;
		emiterInfo->Attribute("Value", &imageSizW);

		emiterInfo = startPoint->FirstChildElement("ImageSizeHeight");
		int imageSizH;
		emiterInfo->Attribute("Value", &imageSizH);

		emiterInfo = startPoint->FirstChildElement("MaxParticles");
		int maxParti;
		emiterInfo->Attribute("Value", &maxParti);

		if (maxParti < 0)
			return false;

		emiterInfo = startPoint->FirstChildElement("FirstWave");
		int firstWav;
		emiterInfo->Attribute("Value", &firstWav);

		if (firstWav > maxParti)
			return false;

		emiterInfo = startPoint->FirstChildElement("SpawnRate");
		double spawnRa;
		emiterInfo->Attribute("Value", &spawnRa);

		emiterInfo = startPoint->FirstChildElement("Shape");
		int mshape;
		emiterInfo->Attribute("Value", &mshape);

		 emiterInfo = startPoint->FirstChildElement("Flyweight");

		 TiXmlElement* flyweightInfo = emiterInfo->FirstChildElement("MinLife");
		 double minlife;
		 flyweightInfo->Attribute("Value", &minlife);
		 if (minlife < 0)
			 return false;

		 flyweightInfo = emiterInfo->FirstChildElement("MaxLife");
		 double maxlife;
		 flyweightInfo->Attribute("Value", &maxlife);
		 if (minlife > maxlife)
			 return false;

		 flyweightInfo = emiterInfo->FirstChildElement("ColorStartA");
		 int colSA;
		 flyweightInfo->Attribute("Value", &colSA);
		 if (colSA > 255)
			 return false;

		 flyweightInfo = emiterInfo->FirstChildElement("ColorStartR");
		 int colSR;
		 flyweightInfo->Attribute("Value", &colSR);
		 if (colSR > 255)
			 return false;

		 flyweightInfo = emiterInfo->FirstChildElement("ColorStartG");
		 int colSG;
		 flyweightInfo->Attribute("Value", &colSG);
		 if (colSG > 255)
			 return false;

		 flyweightInfo = emiterInfo->FirstChildElement("ColorStartB");
		 int colSB;
		 flyweightInfo->Attribute("Value", &colSB);
		 if (colSB > 255)
			 return false;

		 flyweightInfo = emiterInfo->FirstChildElement("ColorEndA");
		 int colEA;
		 flyweightInfo->Attribute("Value", &colEA);
		 if (colEA > 255)
			 return false;

		 flyweightInfo = emiterInfo->FirstChildElement("ColorEndR");
		 int colER;
		 flyweightInfo->Attribute("Value", &colER);
		 if (colER > 255)
			 return false;

		 flyweightInfo = emiterInfo->FirstChildElement("ColorEndG");
		 int colEG;
		 flyweightInfo->Attribute("Value", &colEG);
		 if (colEG > 255)
			 return false;

		 flyweightInfo = emiterInfo->FirstChildElement("ColorEndB");
		 int colEB;
		 flyweightInfo->Attribute("Value", &colEB);
		 if (colEB > 255)
			 return false;

		 flyweightInfo = emiterInfo->FirstChildElement("ScaleStart");
		 double scalS;
		 flyweightInfo->Attribute("Value", &scalS);
		 if (scalS < 0 )
			 return false;

		 flyweightInfo = emiterInfo->FirstChildElement("ScaleEnd");
		 double scalE;
		 flyweightInfo->Attribute("Value", &scalE);
		 if (scalE < 0)
			 return false;

		 flyweightInfo = emiterInfo->FirstChildElement("RotationStart");
		 double rotS;
		 flyweightInfo->Attribute("Value", &rotS);

		 rotS = (rotS * SGD::PI) / 180;
		 //if (rotS < 0)
			// return false;

		 flyweightInfo = emiterInfo->FirstChildElement("RotationEnd");
		 double rotE;
		 flyweightInfo->Attribute("Value", &rotE);
		 rotE = (rotE * SGD::PI) / 180;
		 //if (rotE < 0)
			// return false;

		 flyweightInfo = emiterInfo->FirstChildElement("VelocityStartMinX");
		 double velSminX;
		 flyweightInfo->Attribute("Value", &velSminX);

		 flyweightInfo = emiterInfo->FirstChildElement("VelocityStartMaxX");
		 double velSmaxX;
		 flyweightInfo->Attribute("Value", &velSmaxX);

		 flyweightInfo = emiterInfo->FirstChildElement("VelocityStartMinY");
		 double velSminY;
		 flyweightInfo->Attribute("Value", &velSminY);

		 flyweightInfo = emiterInfo->FirstChildElement("VelocityStartMaxY");
		 double velSmaxY;
		 flyweightInfo->Attribute("Value", &velSmaxY);

		 flyweightInfo = emiterInfo->FirstChildElement("VelocityEndMinX");
		 double velEminX;
		 flyweightInfo->Attribute("Value", &velEminX);

		 flyweightInfo = emiterInfo->FirstChildElement("VelocityEndMaxX");
		 double velEmaxX;
		 flyweightInfo->Attribute("Value", &velEmaxX);

		 flyweightInfo = emiterInfo->FirstChildElement("VelocityEndMinY");
		 double velEminY;
		 flyweightInfo->Attribute("Value", &velEminY);

		 flyweightInfo = emiterInfo->FirstChildElement("VelocityEndMaxY");
		 double velEmaxY;
		 flyweightInfo->Attribute("Value", &velEmaxY);

		 ParticleFlyweight* fly = new ParticleFlyweight((float)minlife, (float)maxlife, (unsigned char)colSA, (unsigned char)colSR, (unsigned char)colSG, (unsigned char)colSB,
			 (unsigned char)colEA, (unsigned char)colER, (unsigned char)colEG, (unsigned char)colEB, (float)scalS, (float)scalE, (float)rotS, (float)rotE, (float)velSminX,
			 (float)velSmaxX, (float)velEminX, (float)velEmaxX, (float)velSminY, (float)velSmaxY, (float)velEminY, (float)velEmaxY);

		
		 //string convertion
		 size_t newsize = strlen(imagePath) + 1;
		 wchar_t * wcstring = new wchar_t[newsize];
		 size_t convertedChars = 0;

		 mbstowcs_s(&convertedChars, wcstring, newsize, imagePath, _TRUNCATE);

		 //delete[] wcstring;
		 SGD::String aux;
		 aux = wcstring;
		 delete wcstring;

		 Emiter* emi = new Emiter(aux, SGD::Point{ (float)positionx, (float)positiony }, (float)width, (float)height, SGD::Size{ (float)width, (float)height },
			 (unsigned int)maxParti, (unsigned int)firstWav, (float)spawnRa, SGD::Size{ (float)imageSizW, (float)imageSizH }, mshape, fly);

		 AddEmiter(emi, 0);
		 delete fly;
	}
	return true;
}