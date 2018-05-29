#include "PhaseEnd.h"
#include "Character.h"
#include "Map.h"

PhaseEnd::PhaseEnd( std::list< CharacterPtr > &chara, int spy_idx, MapPtr map ) :
_map( map ) {
	std::list< CharacterPtr >::iterator ite;
	ite = chara.begin( );
	for ( int i = 0; i < spy_idx; i++ ) {
		ite++;
	}
	_spy = ( *ite );
	_spy->setFixedpoint( PHASE_END );
	_endpoint = _map->getFixedpointBeta( PHASE_END );
}

PhaseEnd::~PhaseEnd( ) {
}

void PhaseEnd::update( ) {
}

void PhaseEnd::draw( ) const {
}