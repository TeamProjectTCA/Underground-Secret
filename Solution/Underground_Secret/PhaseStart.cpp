#include "PhaseStart.h"
#include "Character.h"
#include "Keyboard.h"
#include "Map.h"
#include "const.h"

const int START_X = 0;
const int START_Y = 100;
const double MOVE = 5;
const int OPEN_ANIMATION_TIME = ONE_SECOND_FRAME * 3;

PhaseStart::PhaseStart( std::list< CharacterPtr > &chara, MapPtr map ) :
_chara( chara ),
_map( map ) {
	_keyboard = Keyboard::getTask( );

	std::list< CharacterPtr >::iterator ite;
	ite = _chara.begin( );
	for ( ite; ite != _chara.end( ); ite++ ) {
		( *ite )->setFixedpoint( PHASE_START );
	}

	_end_point = _map->getFixedpointBeta( PHASE_START );
	_run_idx = 0;
	_run_chara = ( *_chara.begin( ) );
	_open_animation_count = 0;
	_open_animation = false;
}

PhaseStart::~PhaseStart( ) {
}

void PhaseStart::update( ) {
	if ( _run_idx < ( int )_chara.size( ) ) {
		// betaポイントよりxが小さければ移動
		if ( _run_chara->getPos( ).x < _end_point.x ) {
			_run_chara->move( Vector( MOVE, 0 ) );
		} else {
		// betaポイントに到達したらopenアニメーション
			if ( !_open_animation ) {
				_run_chara->setAnim( Character::OPEN );
				_open_animation = true;
			}
			_open_animation_count++;

			// アニメーション終了
			if ( _open_animation_count > OPEN_ANIMATION_TIME ) {
				_open_animation_count = 0;
				_open_animation = false;
				_run_idx++;
				_run_chara->setFixedpoint( PHASE_PLAY );
				changeRunCharacter( );
				return;
			}
		}

		_run_chara->draw( );
	} else {
		// フェイズを変える
		setPhase( PHASE_PLAY );
	}
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