#include "PhasePlay.h"
#include "Character.h"
#include "Drawer.h"
#include "Sound.h"
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

const int TIME_LIMIT = 60;

PhasePlay::PhasePlay( std::list< CharacterPtr > &chara, int spy_idx, ScrollPtr scroll ) :
_chara( chara ),
_scroll( scroll ) {
	_drawer = Drawer::getTask( );
	_sound = Sound::getTask( );
	_time_count = TIME_LIMIT * FPS;

	//BGM
	_play_bgm = _sound->load( "GameBGM/gamebgm1.ogg" );
	_sound->play( _play_bgm, true );

	//SE
	_win_se = _sound->load( "SoundEffect/win.ogg" );
	_lose_se = _sound->load( "SoundEffect/lose.ogg" );

	std::list< CharacterPtr >::iterator ite;
	ite = _chara.begin( );
	for ( int i = 0; i < spy_idx; i++ ) {
		ite++;
	}
	_spy = ( *ite );

	_profiling = ProfilingPtr( new Profiling( _spy->getInfo( ) ) );
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

	_time_count--;

	if ( isInvasion( ) ) {
		_sound->stop( _play_bgm );
		_sound->play( _lose_se );
		setPhase( PHASE_END );
	}

	if ( isClear( ) ) {
		_sound->stop( _play_bgm );
		_sound->play( _win_se );
		setPhase( PHASE_END );
	}
}

bool PhasePlay::isClear( ) {
	if ( _time_count <= 0 ) {
		return true;
	}
	return false;
}

bool PhasePlay::isInvasion( ) const {
	if ( _spy->getMapData( _spy->getPos( ) ) == IDENTIFICATION_ENDPOINT ) {
		return true;
	}
	return false;
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
