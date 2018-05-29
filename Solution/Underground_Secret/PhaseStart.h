#pragma once
#include "smart_ptr.h"
#include "Phase.h"
#include <list>
#include <vector>
#include "Vector.h"

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
	void draw( ) const;

private:
	void changeRunCharacter( );

private:
	std::list< CharacterPtr > _chara;
	Vector _endpoint;
	int _run_idx;
	int _open_animation_count;
	bool _open_animation;

	CharacterPtr _run_chara;
	KeyboardPtr _keyboard;
	MapPtr _map;
};

