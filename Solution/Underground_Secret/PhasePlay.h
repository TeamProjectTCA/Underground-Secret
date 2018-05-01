#pragma once
#include "Phase.h"
#include "smart_ptr.h"

PTR( PhasePlay );

class PhasePlay : public Phase {
public:
	PhasePlay( );
	virtual ~PhasePlay( );

public:
	void update( );
};

