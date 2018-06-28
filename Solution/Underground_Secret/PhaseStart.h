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
PTR( SpecialScroll );
PTR( SpecialElevator );

class PhaseStart : public Phase {
public:
	PhaseStart( std::list< CharacterPtr > &chara, MapPtr map, SpecialScrollPtr scroll );
	virtual ~PhaseStart( );

public:
	void update( );
	void draw( ) const;

private:
	void changeRunCharacter( );

private:
	std::list< CharacterPtr > _chara;
	Vector _entrance_point;
	Vector _playpoint;
	Vector _move;
	Vector _focus_move;
	int _count;
	int _focus_count;

	int _run_idx;

	CharacterPtr _run_chara;
	KeyboardPtr _keyboard;
	MapPtr _map;
	SpecialScrollPtr _scroll;
	SpecialElevatorPtr _elevator;
};

