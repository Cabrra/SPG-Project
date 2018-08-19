#pragma once
class Trap
{
public:
	enum TrapType {
		TRAP_FIRE,
		TRAP_LIGHTING, TRAP_ARROW,
		TRAP_SPIKE, TRAP_ICE
	};
	Trap();
	~Trap();
};

