#include "CharaDummy.h"
#include "Drawer.h"
#include "const.h"
#include "Debug.h"
#include "Infomation.h"
#include "Random.h"

const int MOVE_RATE_X = 3;
const int MOVE_RATE_Y = BLOCK_SIZE;
const float RANDOM_PROBABILITY = 0.3f;

CharaDummy::CharaDummy( MapPtr map, InfomationPtr info ) :
Character( map, info->getInfo( CHARA_DUMMY ) ),
_ride_elevator( false ),
_return_move( false ) {
	_random = RandomPtr( new Random( ) );

	addAnim( Character::WALK, "CharaDummy_Walk", 2 );
	addAnim( Character::OPEN, "CharaDummy_Open", 2 );
	setAnim( Character::WALK );

	_dir = MOVE_RIGHT;
	setDistance( );
}

CharaDummy::~CharaDummy( ) {
}

void CharaDummy::update( ) {
	setScroll( );
	walk( );
	fall( );
	countLooking( );
	checkCaughtCollider( );

	// debug
	if ( _debug->isDebug( ) ) {
		_debug->addLog( "監視時間:" + std::to_string( _looking_time / FPS ) + "秒" );
	}
}

void CharaDummy::walk( ) {
	if ( _dir == MOVE_DOWN ) {
		return;
	}
	// 進行方向に予測座標をセット
	setDistance( );

	checkCollider( );
	checkElevator( );
}

void CharaDummy::fall( ) {
	MOVE_DIRECTION past = _dir;
	_dir = MOVE_DOWN;

	// 進行方向に予測座標をセット
	setDistance( );

	// 横方向に進行方向を戻す
	_dir = past;

	bool move_ok = true;
	
	int data = getMapDataCollider( getPos( ) + _distance );
	if ( data < 0 ) {
		return;
	}
	if ( data != IDENTIFICATION_NONE ) {
		move_ok = false;
	}

	if ( !move_ok ) {
		return;
	}

	// 移動させる
	setFallPos( getPos( ) + _distance );
}

void CharaDummy::setDistance( ) {
	switch ( _dir ) {
	case MOVE_RIGHT: _distance = Vector(  MOVE_RATE_X, 0 ); break;
	case MOVE_LEFT : _distance = Vector( -MOVE_RATE_X, 0 ); break;
	case MOVE_DOWN : _distance = Vector(  0, MOVE_RATE_Y ); break;
	}
}

void CharaDummy::countLooking( ) {
	if ( isLooking( getPos( ) ) ) {
		_looking_time++;
	}
}

void CharaDummy::checkCollider( ) {
	// アスキーコードを取得
	int data = getMapDataCollider( getPos( ) + _distance );

	// エレベーターだったら
	if ( data < 0 ) {
		return;
	}

	// 進行予測値が当たり判定であったら
	if ( data == IDENTIFICATION_COLLIDER ) {
		// もう1つ上を見る
		data = getMapDataCollider( getPos( ) + _distance + Vector( 0, -BLOCK_SIZE ) );
		if ( data == IDENTIFICATION_COLLIDER ) {
			// 2回目も当たり判定があったら
			_return_move = true;
		} else {
			_distance += Vector( 0, -BLOCK_SIZE );
		}
	}

	// 方向転換
	if ( _return_move ) {
		returnMove( );
		setDistance( );
		_return_move = false;
	}

	// 移動させる	
	move( _distance );
}

void CharaDummy::checkElevator( ) {
	// アスキーコードを取得
	int data = getMapDataElevator( getPos( ) );

	// エレベーター以外だったら
	if ( data < 0 ) {
		_ride_elevator = false;
		return;
	}

	// 乗るか乗らないかの抽選
	if ( _random->getRealOne( ) > RANDOM_PROBABILITY && !_ride_elevator ) {
		_ride_elevator = true;
	}

	// 連続で乗るのを防ぐ
	if ( _ride_elevator ) {
		return;
	}

	setElevatorPos( data );
	_ride_elevator = true;
}

void CharaDummy::returnMove( ) {
	switch ( _dir ) {
	case MOVE_RIGHT: _dir = MOVE_LEFT ; break;
	case MOVE_LEFT : _dir = MOVE_RIGHT; break;
	default: return;
	}
}