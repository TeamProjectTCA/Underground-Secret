#include "PhasePlay.h"
#include "Character.h"
#include "Drawer.h"
#include "const.h"
#include "Scroll.h"
#include <string>

const float TIME_BOX_X = 10;
const float TIME_BOX_Y = 10;
const float TIME_BOX_WIDTH = 140;
const float TIME_BOX_HEIGHT = 70;
const float TIME_STRING_X1 = 20;
const float TIME_STRING_Y1 = 20;
const float TIME_STRING_X2 = 60;
const float TIME_STRING_Y2 = 50;

PhasePlay::PhasePlay( std::list< CharacterPtr > &chara, ScrollPtr scroll ) :
_chara( chara ),
_scroll( scroll ) {
	_drawer = Drawer::getTask( );
	_time_count = FINISH_TIME * ONE_SECOND_FRAME;
}

PhasePlay::~PhasePlay( ) {
}

void PhasePlay::update( ) {
	_scroll->update( );
	countClear( );
	setEnd( );
	drawTime( );
}

void PhasePlay::countClear( ) {
	_time_count--;
	if ( _time_count <= 0 ) {
		_time_count = 0;
		setPhase( PHASE_END );
	}
}

void PhasePlay::setEnd( ) {
	std::list< CharacterPtr >::iterator ite = _chara.begin( );
	for ( ite; ite != _chara.end( ); ite++ ) {
		if ( ( *ite )->isEndpoint( ( *ite )->getPos( ) ) ) {
			setPhase( PHASE_END );
		}
	}
}

void PhasePlay::drawTime( ) {
	_drawer->drawBox( TIME_BOX_X, TIME_BOX_Y, TIME_BOX_X + TIME_BOX_WIDTH, TIME_BOX_Y + TIME_BOX_HEIGHT, BLUE, true );
	_drawer->drawString( TIME_STRING_X1, TIME_STRING_Y1, "ƒNƒŠƒA‚Ü‚Å‚ ‚Æ", RED );
	_drawer->drawString( TIME_STRING_X2, TIME_STRING_Y2, std::to_string( _time_count / ONE_SECOND_FRAME ) + "•b", RED );
}
