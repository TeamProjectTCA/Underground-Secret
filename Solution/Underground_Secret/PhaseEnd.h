#pragma once
#include "Phase.h"
#include "smart_ptr.h"
#include "Vector.h"
#include <list>

PTR( PhaseEnd );
PTR( Character );
PTR( Map );
PTR( Shutter );

class PhaseEnd : public Phase {
public:
	PhaseEnd( std::list< CharacterPtr > &chara, MapPtr map );
	virtual ~PhaseEnd( );

public:
	void update( );
	void draw( ) const;

private:
	Vector _endpoint;
	Vector _scroll;
	Vector _move;
	Vector _focus_move;
	int _count;
	int _focus_count;

	CharacterPtr _spy;
	MapPtr _map;
	ShutterPtr _shutter;
};

