#include "PhasePlay.h"
#include "Character.h"
#include "Drawer.h"
#include "Sound.h"
#include "const.h"
#include "Scroll.h"
#include "Profiling.h"
#include "Random.h"
#include <string>

const float TIME_BOX_X = 10;
const float TIME_BOX_Y = 10;
const float TIME_BOX_WIDTH = 140;
const float TIME_BOX_HEIGHT = 70;
const float TIME_STRING_X1 = 20;
const float TIME_STRING_Y1 = 20;
const float TIME_STRING_X2 = 60;
const float TIME_STRING_Y2 = 50;

const int TIME_LIMIT = 300;

PhasePlay::PhasePlay( std::list< CharacterPtr > &chara, ScrollPtr scroll ) :
_chara( chara ),
_scroll( scroll ),
_countdown( false ) {
	_drawer = Drawer::getTask( );
	_sound = Sound::getTask( );
	_time_count = TIME_LIMIT * FPS;

	//タイムボード
	_time_board_handle = _drawer->getImage( "time_board" );

	//BGM
	_sound_handle[ GAME_BGM ] = _sound->load( "GameBGM/gamebgm.ogg" );
	_sound->play( _sound_handle[ GAME_BGM ], true, true );


	std::list< CharacterPtr >::iterator ite;
	ite = _chara.begin( );
	for ( ite; ite != _chara.end( ); ite++ ) {
		if ( ( *ite )->isSpy( ) ) {
			break;
		}
	}
	_spy = ( *ite );

	_profiling = ProfilingPtr( new Profiling( _spy->getInfo( ) ) );

	_gameover_count = Random::getTask( )->getInt32( ( unsigned long )FPS, ( unsigned long )FPS * 10 );
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


	if ( _spy->getMapData( _spy->getPos( ) ) == IDENTIFICATION_ENDPOINT && !_countdown ) {
		_countdown = true;
		_spy->setMoveFlag( false );
		_spy->setAnim( Character::ANIM_WAIT_ELEVATOR );
	}

	if ( _countdown ) {
		_gameover_count--;
	}

	if ( isInvasion( ) ) {
		setResult( LOSE );
		setPhase( PHASE_END );
		_sound->stop( _sound_handle[ GAME_BGM ] );
	}

	if ( isClear( ) ) {
		setResult( WIN );
		setPhase( PHASE_END );
		_sound->stop( _sound_handle[ GAME_BGM ] );
	}
}

bool PhasePlay::isClear( ) const {
	if ( _time_count < 1 ) {
		return true;
	}
	return false;
}

bool PhasePlay::isInvasion( ) const {
	if ( _gameover_count < 1 ) {
		_sound->play( _sound->load( "SoundEffect/ElevatorArrive1.ogg" ), false, true );
		return true;
	}
	return false;
}

void PhasePlay::draw( ) const {
	_profiling->draw( );
	drawTime( );
}

void PhasePlay::drawTime( ) const {
	short int tmp = _time_count / FPS;
	const float NUM_WIDTH = 32;
	const float NUM_HEIGHT = 32;
	const float NUM_X1 = 65;
	const float NUM_X2 = 52;
	const float NUM_X3 = 40;
	const float NUM_Y = 40;
	const float NUM_MARGIN = 25;
	std::string index = std::to_string( tmp );

	std::vector< int > number;
	std::vector< int > frame;
	short int length = ( short int )index.length( );

	for ( int i = 0; i < length; i++ ) {
		short int handle_idx = tmp % 10;
		number.push_back( _drawer->getImage( std::to_string( handle_idx ) ) );
		frame.push_back( _drawer->getImage( std::to_string( handle_idx ) + "frame" ) );
		tmp /= 10;
	}

	_drawer->drawGraph( TIME_BOX_X, TIME_BOX_Y, _time_board_handle, false );
	for ( int i = 0; i < length; i++ ) {
		if ( length == 3 ) {
			_drawer->drawExtendGraph( NUM_X3 + i * NUM_MARGIN, NUM_Y, NUM_X3 + i * NUM_MARGIN + NUM_WIDTH, NUM_Y + NUM_HEIGHT, number[ length - i - 1 ], true );
			_drawer->drawExtendGraph( NUM_X3 + i * NUM_MARGIN, NUM_Y, NUM_X3 + i * NUM_MARGIN + NUM_WIDTH, NUM_Y + NUM_HEIGHT, frame[ length - i - 1 ], true );
		}
		if ( length == 2 ) {
			_drawer->drawExtendGraph( NUM_X2 + i * NUM_MARGIN, NUM_Y, NUM_X2 + i * NUM_MARGIN + NUM_WIDTH, NUM_Y + NUM_HEIGHT, number[ length - i - 1 ], true );
			_drawer->drawExtendGraph( NUM_X2 + i * NUM_MARGIN, NUM_Y, NUM_X2 + i * NUM_MARGIN + NUM_WIDTH, NUM_Y + NUM_HEIGHT, frame[ length - i - 1 ], true );
		}
		if ( length == 1 ) {
			_drawer->drawExtendGraph( NUM_X1, NUM_Y, NUM_X1 + NUM_WIDTH, NUM_Y + NUM_HEIGHT, number[ length - i - 1 ], true );
			_drawer->drawExtendGraph( NUM_X1, NUM_Y, NUM_X1 + NUM_WIDTH, NUM_Y + NUM_HEIGHT, frame[ length - i - 1 ], true );
		}
	}
}