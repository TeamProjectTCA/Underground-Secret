#pragma once
#include "smart_ptr.h"
#include "Phase.h"

PTR( PhaseStart );

class PhaseStart : public Phase {
public:
	PhaseStart( );
	virtual ~PhaseStart( );

public:
	void update( );
};

