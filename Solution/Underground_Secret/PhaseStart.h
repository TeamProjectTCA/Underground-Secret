#pragma once
#include "smart_ptr.h"
#include "Phase.h"
#include <list>

PTR( PhaseStart );
PTR( Character );
PTR( Keyboard );

class PhaseStart : public Phase {
public:
	PhaseStart( std::list< CharacterPtr > &chara );
	virtual ~PhaseStart( );

public:
	void update( );

private:
	std::list< CharacterPtr > _chara;

	KeyboardPtr _keyboard;
};

