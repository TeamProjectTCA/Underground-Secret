#include "PhaseEnd.h"
#include "Character.h"
#include "Map.h"
#include "const.h"

const float MOVE_FRAME = FPS * 3;
const float PERFORMANCE_TIME = FPS * 10;

PhaseEnd::PhaseEnd( std::list< CharacterPtr > &chara, int spy_idx, MapPtr map ) :
_map( map ),
_count( 0 ) {
	std::list< CharacterPtr >::iterator ite;
	ite = chara.begin( );
	for ( int i = 0; i < spy_idx; i++ ) {
		ite++;
	}
	_spy = ( *ite );
	_spy->setFixedpoint( PHASE_END );
	_spy->setAnimTime( FPS );
	_endpoint = _map->getFixedpointBeta( PHASE_END );
	const float SPEED = ( float )( _endpoint - _spy->getPos( ) ).getLength( ) * ( 1 / MOVE_FRAME );
	_move = ( _endpoint - _spy->getPos( ) ).normalize( ) * SPEED;
}

PhaseEnd::~PhaseEnd( ) {
}

void PhaseEnd::update( ) {
	// ��_�܂ňړ�
	if ( _count < MOVE_FRAME ) {
		_count++;
		_spy->move( _move );
		return;
	}

	if ( _count == MOVE_FRAME ) {
		// �A�j���[�V����
		_spy->setAnim( Character::ANIM_OPEN );
	}
	_count++;

	if ( _count > PERFORMANCE_TIME ) {
		setPhase( PHASE_RETURN_TITLE );
	}
}

void PhaseEnd::draw( ) const {
	_spy->draw( );
}