#pragma once
#include "smart_ptr.h"

PTR( Phase );

enum PHASE {
	PHASE_START,
	PHASE_PLAY,
	PHASE_END,
	PHASE_RETURN_TITLE,
	PHASE_MAX,
	PHASE_CONTINUE,
};

class Phase {
public:
	Phase( );
	virtual ~Phase( );

public:
	virtual void update( ) = 0;
	virtual void draw( ) const;
	virtual void setPhase( PHASE phase );
	virtual PHASE getPhase( ) const;

private:
	PHASE _phase;
};

