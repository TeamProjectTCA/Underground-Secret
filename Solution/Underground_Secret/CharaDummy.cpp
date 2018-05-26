#include "CharaDummy.h"
#include "Drawer.h"
#include "const.h"
#include "Debug.h"
#include "Infomation.h"

const int MOVE_RATE_X = 3;
const int MOVE_RATE_Y = BLOCK_SIZE;

CharaDummy::CharaDummy( MapPtr map, InfomationPtr info ) :
Character( map, info->getInfo( CHARA_DUMMY ) ) {
	_debug = Debug::getTask( );

	addAnim( Character::WALK, "CharaDummy_Walk", 2 );
	addAnim( Character::OPEN, "CharaDummy_Open", 2 );
	setAnim( Character::WALK );

	_dir = MOVE_RIGHT;
	_ride_elevator = false;
	setDistance( );
}

CharaDummy::~CharaDummy( ) {
}

void CharaDummy::update( ) {
	_dir = MOVE_RIGHT;
	walk( );
	fall( );
	countLooking( );
	checkCaughtCollider( );

	if ( _debug->isDebug( ) ) {
		_drawer->drawString( 10, 100, "監視時間" + std::to_string( _looking_time / FPS ) + "秒", RED );
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
	_dir = MOVE_DOWN;

	// 進行方向に予測座標をセット
	setDistance( );
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
	bool move_ok = true;

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
			move_ok = false;
		} else {
			_distance += Vector( 0, -BLOCK_SIZE );
		}
	}

	if ( !move_ok ) {
		return;
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

	// 連続で乗るのを防ぐ
	if ( _ride_elevator ) {
		return;
	}

	setElevatorPos( data );
	_ride_elevator = true;
}