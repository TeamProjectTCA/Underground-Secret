#pragma once
#include "Phase.h"
#include "smart_ptr.h"

PTR( PhaseEnd );

class PhaseEnd : public Phase {
public:
	PhaseEnd( );
	virtual ~PhaseEnd( );

public:
	void update( );
};

