#pragma once
#include "Phase.h"
#include "smart_ptr.h"
#include "Vector.h"
#include <list>

PTR( PhaseEnd );
PTR( Character );
PTR( Map );

class PhaseEnd : public Phase {
public:
	PhaseEnd( std::list< CharacterPtr > &chara, int spy_idx, MapPtr map );
	virtual ~PhaseEnd( );

public:
	void update( );
	void draw( ) const;

private:
	Vector _endpoint;

	CharacterPtr _spy;
	MapPtr _map;
};

