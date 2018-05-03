#include "PhasePlay.h"
#include "Drawer.h"
#include "CharaA.h"
#include "const.h"
#include <string>

PhasePlay::PhasePlay( ) {
	_drawer = Drawer::getTask( );
	_chanaA = CharaAPtr( new CharaA( ) );
	_time_count = FINISH_TIME * FRAME;
}

PhasePlay::~PhasePlay( ) {
}

void PhasePlay::update( ) {
	countClear( );
	drawTime( );
	_chanaA->update( );
}

void PhasePlay::countClear( ) {
	_time_count--;
	if ( _time_count <= 0 ) {
		_time_count = 0;
		setPhase( PHASE_END );
	}
}

void PhasePlay::drawTime( ) {
	const float TIME_BOX_X = 10;
	const float TIME_BOX_Y = 10;
	const float TIME_BOX_WIDTH = 140;
	const float TIME_BOX_HEIGHT = 70;
	const float TIME_STRING_X1 = 20;
	const float TIME_STRING_Y1 = 20;
	const float TIME_STRING_X2 = 60;
	const float TIME_STRING_Y2 = 50;
	_drawer->drawBox( TIME_BOX_X, TIME_BOX_Y, TIME_BOX_X + TIME_BOX_WIDTH, TIME_BOX_Y + TIME_BOX_HEIGHT, 0x0000ff, false );
	_drawer->drawString( TIME_STRING_X1, TIME_STRING_Y1, "クリアまであと", 0xff0000 );
	_drawer->drawString( TIME_STRING_X2, TIME_STRING_Y2, std::to_string( _time_count / FRAME ) + "秒", 0xff0000 );
}
