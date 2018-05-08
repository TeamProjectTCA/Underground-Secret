#include "PhaseStart.h"
#include "Character.h"
#include "Keyboard.h"

PhaseStart::PhaseStart( std::list< CharacterPtr > &chara ) :
_chara( chara ) {
	_keyboard = Keyboard::getTask( );
}

PhaseStart::~PhaseStart( ) {
}

void PhaseStart::update( ) {
	if ( _keyboard->getKeyDown( "ENTER" ) ) {
		std::list< CharacterPtr >::iterator ite;
		ite = _chara.begin( );
		for ( ite; ite != _chara.end( ); ite++ ) {
			( *ite )->addAnim( Character::WALK, "charaA", 2 );
		}
		setPhase( PHASE_PLAY );
	}
}