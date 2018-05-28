#include "PhasePlay.h"
#include "Character.h"
#include "Drawer.h"
#include "const.h"
#include "Scroll.h"
#include "Profiling.h"
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
	_profiling = ProfilingPtr( new Profiling( ) );
	_time_count = FINISH_TIME * FPS;
}

PhasePlay::~PhasePlay( ) {
}

void PhasePlay::update( ) {
	// プロファイリング
	_profiling->update( );

	// スクロール
	if ( !_profiling->isActive( ) ) {
		_scroll->update( );
	}

	countClear( );
	setEnd( );
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

void PhasePlay::draw( ) const {
	_profiling->draw( );
	drawTime( );
}

void PhasePlay::drawTime( ) const {
	_drawer->drawBox( TIME_BOX_X, TIME_BOX_Y, TIME_BOX_X + TIME_BOX_WIDTH, TIME_BOX_Y + TIME_BOX_HEIGHT, BLUE, true );
	_drawer->drawString( TIME_STRING_X1, TIME_STRING_Y1, "クリアまであと", RED );
	_drawer->drawString( TIME_STRING_X2, TIME_STRING_Y2, std::to_string( _time_count / FPS ) + "秒", RED );
}
