#include "PhaseStart.h"
#include "Character.h"
#include "Keyboard.h"
#include "Map.h"
#include "const.h"

const int START_X = 0;
const int START_Y = 100;
const double MOVE = 5;
const int OPEN_ANIMATION_TIME = FPS * 3;

PhaseStart::PhaseStart( std::list< CharacterPtr > &chara, MapPtr map ) :
_chara( chara ),
_map( map ) {
	_keyboard = Keyboard::getTask( );

	std::list< CharacterPtr >::iterator ite;
	ite = _chara.begin( );
	for ( ite; ite != _chara.end( ); ite++ ) {
		( *ite )->setFixedpoint( PHASE_START );
	}

	_endpoint = _map->getFixedpointBeta( PHASE_START );
	_run_idx = 0;
	_run_chara = ( *_chara.begin( ) );
	_open_animation_count = 0;
	_open_animation = false;
}

PhaseStart::~PhaseStart( ) {
}

void PhaseStart::update( ) {
	// 全キャラの演出終了で遷移
	if ( _run_idx >= ( int )_chara.size( ) ) {
		setPhase( PHASE_PLAY );
		return;
	}

	// betaポイントよりxが小さければ移動
	if ( _run_chara->getPos( ).x < _endpoint.x ) {
		_run_chara->move( Vector( MOVE, 0 ) );
		return;
	}

	// betaポイントに到達したらopenアニメーション
	if ( !_open_animation ) {
		_run_chara->setAnim( Character::ANIM_OPEN );
		_open_animation = true;
	}
	_open_animation_count++;

	// アニメーション終了
	if ( _open_animation_count > OPEN_ANIMATION_TIME ) {
		_open_animation_count = 0;
		_open_animation = false;
		_run_idx++;
		_run_chara->setFixedpoint( PHASE_PLAY );
		_run_chara->setAnim( Character::ANIM_WALK );
		changeRunCharacter( );
		return;
	}
}

void PhaseStart::draw( ) const {
	_run_chara->draw( );
}

void PhaseStart::changeRunCharacter( ) {
	if ( _run_idx >= ( int )_chara.size( ) ) {
		return;
	}

	std::list< CharacterPtr >::iterator ite;
	ite = _chara.begin( );
	for ( int i = 0; i < _run_idx; i++ ) {
		ite++;
	}

	_run_chara = ( *ite );
}