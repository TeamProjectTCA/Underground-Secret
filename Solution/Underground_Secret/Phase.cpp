#include "Phase.h"

Phase::Phase( ) {
	_phase = PHASE_CONTINUE;
	_result = NONE;
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


void Phase::setResult( RESULT result ) {
	_result = result;
}

RESULT Phase::getResult( ) const {
	return _result;
}