#include "Phase.h"

Phase::Phase( ) {
	_phase = PHASE_CONTINUE;
}

Phase::~Phase( ) {
}

void Phase::draw( ) const {
}

void Phase::setPhase( PHASE phase ) {
	_phase = phase;
}

PHASE Phase::getPhase( ) const {
	return _phase;
}