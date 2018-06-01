#include "PhaseEnd.h"
#include "Character.h"
#include "Map.h"
#include "const.h"

const float MOVE_FRAME = FPS * 3;
const float PERFORMANCE_TIME = FPS * 2;

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
	_count++;

	// 定点まで移動
	if ( _count < MOVE_FRAME ) {
		_spy->move( _move );
		return;
	}

	if ( _count == MOVE_FRAME ) {
		// アニメーション
		_spy->setAnim( Character::ANIM_OPEN );
	}

	if ( _count == MOVE_FRAME + PERFORMANCE_TIME ) {
		setPhase( PHASE_RETURN_TITLE );
	}
}

void PhaseEnd::draw( ) const {
	_spy->draw( );
}