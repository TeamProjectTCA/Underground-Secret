#include "PhaseStart.h"
#include "Character.h"
#include "Keyboard.h"

const int START_X = 0;
const int START_Y = 100;

PhaseStart::PhaseStart( std::list< CharacterPtr > &chara ) :
_chara( chara ) {
	_keyboard = Keyboard::getTask( );

	std::list< CharacterPtr >::iterator ite;
	ite = _chara.begin( );
	for ( ite; ite != _chara.end( ); ite++ ) {
		( *ite )->setFixedpoint( PHASE_START );
	}

	_run_idx = 0;
	_run_chara = ( *_chara.begin( ) );
}

PhaseStart::~PhaseStart( ) {
}

void PhaseStart::update( ) {
	_run_chara->draw( );

	int size = ( int )_chara.size( );
	// 今スタート演出中のものがあれば
	if ( _run_idx <= size ) {
		return;
	}

	if ( _keyboard->getKeyDown( "ENTER" ) ) {
		setPhase( PHASE_PLAY );
	}
}