
#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

class Emiter;
#include <vector>		// uses std::vector


/**************************************************************/
// pARTICLEManager class
//	- stores references to game entities
//	- updates & renders all game entities
class ParticleManager
{
public:
	/**********************************************************/
	// Default constructor & destructor
	ParticleManager(void) = default;
	~ParticleManager(void) = default;


	/**********************************************************/
	// Emiter Storage:

	void	AddEmiter(Emiter* pEmiter, unsigned int bucket);
	void	RemoveEmiter(Emiter* pEmiter, unsigned int bucket);
	void	RemoveEmiter(Emiter* pEmiter);
	void	RemoveAll(unsigned int bucket);
	void	RemoveAll(void);

	bool	LoadEmiter(const char* file);


	/**********************************************************/
	// Emiter Upkeep:
	void	UpdateAll(float elapsedTime);
	void	RenderAll(void);

	//void	CheckCollisions(unsigned int bucket1, unsigned int bucket2);

private:
	/**********************************************************/
	// Not a singleton, but still don't want the Trilogy-of-Evil
	ParticleManager(const ParticleManager&) = delete;
	ParticleManager& operator= (const ParticleManager&) = delete;


	/**********************************************************/
	// Typedefs will simplify the templates
	typedef std::vector< Emiter* >			EmiterVector;
	typedef std::vector< EmiterVector >		EmiterTable;

	/**********************************************************/
	// members:
	EmiterTable		m_tEmiters;			// vector-of-vector-of-Emiters* (2D table)
	bool			m_bIterating = false;	// read/write lock
};

#endif //EMITERMANAGER_H
