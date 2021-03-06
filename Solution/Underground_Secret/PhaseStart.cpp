#include "PhaseStart.h"
#include "Character.h"
#include "Keyboard.h"
#include "Map.h"
#include "const.h"
#include "SpecialScroll.h"
#include "SpecialElevator.h"

const float MOVE_FRAME = FPS * 2;
const int PERFORMANCE_TIME = FPS / 2;
const float FOCUS_FRAME = FPS * 1;

PhaseStart::PhaseStart( std::list< CharacterPtr > &chara, MapPtr map, SpecialScrollPtr scroll ) :
_chara( chara ),
_map( map ),
_count( 0 ),
_focus_count( 0 ),
_scroll( scroll ),
_run_idx( 0 ) {
	_keyboard = Keyboard::getTask( );

	Vector start_pos = _map->getFixedpointAlpha( PHASE_START );
	_entrance_point = _map->getFixedpointBeta( PHASE_START );
	_playpoint = _map->getFixedpointAlpha( PHASE_PLAY );

	// elevatorの設定
	_elevator = _map->getSpecialElevatorAlphaPtr( );

	_scroll->setScroll( Vector( -start_pos.x, 0 ) );
	changeRunCharacter( );

	const float SPEED = ( float )( _entrance_point - _run_chara->getPos( ) ).getLength( ) * ( 1 / MOVE_FRAME );
	_move = ( _entrance_point - _run_chara->getPos( ) ).normalize( ) * SPEED;

	const float FOCUS_SPEED = ( float )( ( _playpoint - _entrance_point ).getLength( ) * ( 1 / FOCUS_FRAME ) );
	_focus_move = ( _playpoint - _entrance_point ).normalize( ) * FOCUS_SPEED;

	_elevator->setActiveAlpha( true );
	_elevator->setActiveBeta( true );
}

PhaseStart::~PhaseStart( ) {
}

void PhaseStart::update( ) {
	// 全キャラの演出終了で遷移
	if ( _run_idx >= ( int )_chara.size( ) ) {

		_focus_count++;
		if ( _focus_count < FOCUS_FRAME ) {
			_scroll->setScroll( _focus_move * -1 );
			return;
		}
		setPhase( PHASE_PLAY );
		return;
	}

	_count++;
	if ( FPS / 2 == _count ) {
		_elevator->setActiveAlpha( false );
		_elevator->setActiveBeta( false );
	}

	// 移動
	if ( _count < MOVE_FRAME ) {
		_run_chara->move( _move );
		_run_chara->setScroll( );
	}

	// オープンアニメーションをセット
	if ( _count == MOVE_FRAME ) {
		_run_chara->setAnim( Character::ANIM_RIDE );
	}

	// アニメーション終了
	if ( _count == MOVE_FRAME + PERFORMANCE_TIME ) {
		// アニメーションと位置を設定
		_run_chara->setFixedpoint( PHASE_PLAY );
		_run_chara->setAnim( Character::ANIM_WALK );
		_run_chara->setAnimTime( ANIM_WALK_FRAME );

		// 次の演出キャラに切り替える
		_run_idx++;
		changeRunCharacter( );

		// カウントをリセット
		_count = 0;
	}
}

void PhaseStart::draw( ) const {
	_elevator->draw( );
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
	_run_chara->setFixedpoint( PHASE_START );
	_run_chara->setAnimTime( ANIM_WALK_FRAME );
	_run_chara->setScroll( );
}