#pragma once
#include "smart_ptr.h"
#include "Phase.h"
#include <list>
#include <vector>

PTR( PhaseStart );
PTR( Character );
PTR( Keyboard );
PTR( Map );

class PhaseStart : public Phase {
public:
	PhaseStart( std::list< CharacterPtr > &chara, MapPtr map );
	virtual ~PhaseStart( );

public:
	void update( );

private:
	void changeRunCharacter( );

private:
	std::list< CharacterPtr > _chara;
	int _run_idx;
	int _open_animation_count;
	bool _open_animation;

	CharacterPtr _run_chara;
	KeyboardPtr _keyboard;
	MapPtr _map;
};

