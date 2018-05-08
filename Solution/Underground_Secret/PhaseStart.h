#pragma once
#include "smart_ptr.h"
#include "Phase.h"
#include <list>
#include <vector>

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
	int _run_idx;

	CharacterPtr _run_chara;
	KeyboardPtr _keyboard;
};

