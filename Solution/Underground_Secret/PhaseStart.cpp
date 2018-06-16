#include "PhaseStart.h"
#include "Character.h"
#include "Keyboard.h"
#include "Map.h"
#include "const.h"

const float MOVE_FRAME = FPS * 2;
const int PERFORMANCE_TIME = FPS * 2;

PhaseStart::PhaseStart( std::list< CharacterPtr > &chara, MapPtr map ) :
_chara( chara ),
_map( map ),
_count( 0 ) {
	_keyboard = Keyboard::getTask( );

	std::list< CharacterPtr >::iterator ite;
	ite = _chara.begin( );
	for ( ite; ite != _chara.end( ); ite++ ) {
		( *ite )->setFixedpoint( PHASE_START );
		( *ite )->setAnimTime( FPS / 4 );
	}

	_endpoint = _map->getFixedpointBeta( PHASE_START );
	_run_idx = 0;
	_run_chara = ( *_chara.begin( ) );

	const float SPEED = ( float )( _endpoint - _run_chara->getPos( ) ).getLength( ) * ( 1 / MOVE_FRAME );
	_move = ( _endpoint - _run_chara->getPos( ) ).normalize( ) * SPEED;
}

PhaseStart::~PhaseStart( ) {
}

void PhaseStart::update( ) {
	// 全キャラの演出終了で遷移
	if ( _run_idx >= ( int )_chara.size( ) ) {
		setPhase( PHASE_PLAY );
		return;
	}

	_count++;

	// 移動
	if ( _count < MOVE_FRAME ) {
		_run_chara->move( _move );
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
		_run_chara->setAnimTime( FPS / 2 );

		// 次の演出キャラに切り替える
		_run_idx++;
		changeRunCharacter( );

		// カウントをリセット
		_count = 0;
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