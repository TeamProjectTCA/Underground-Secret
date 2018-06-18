#include "CharaDummy.h"
#include "Drawer.h"
#include "const.h"
#include "Debug.h"
#include "Infomation.h"
#include "Random.h"

const int MOVE_RATE_X = 3;
const int MOVE_RATE_Y = BLOCK_SIZE;
const float RANDOM_PROBABILITY = 0.3f;
const int INFO_SHOWTIME = FPS * 5;
const int CHECK_WAIT_TIME = FPS * 3;
const float WAIT_PROBABILITY = 0.2f;

CharaDummy::CharaDummy( MapPtr map, InfomationPtr info ) :
Character( map, info->getInfo( CHARA_DUMMY ) ),
_ride_elevator( false ),
_return_move( false ),
_looking_time( 0 ),
_wait_count( 0 ),
_wait_ani_time( 0 ),
_wait_time( 0 ) {
	_random = Random::getTask( );

	addAnim( Character::ANIM_WALK, "CharaDummy_Walk", 2 );
	addAnim( Character::ANIM_RIDE, "CharaDummy_Ride", 2 );
	addAnim( Character::ANIM_WAIT, "CharaDummy_Wait", 2 );
	addAnim( Character::ANIM_WAIT_ELEVATOR, "CharaDummy_WaitElevator", 2 );
	setAnim( Character::ANIM_WALK );

	_dir = ( _random->getInt32( 0, 1 ) ? MOVE_RIGHT : MOVE_LEFT );

	setDistance( );
}

CharaDummy::~CharaDummy( ) {
}

void CharaDummy::update( ) {
	setScroll( );

	if ( getAnimType( ) == Character::ANIM_WALK ) {
		walk( );
		fall( );
		checkCaughtCollider( );
	}
	
	if (getAnimType() == Character::ANIM_WAIT) {
		//立ち止まる
		wait();
	}

	// エレベーター
	checkElevator( );

	// 時間経過で情報を表示
	countLooking( );
	if ( _looking_time % INFO_SHOWTIME == 0 ) {
		addShowInfoNum( );
	}

	// debug
	if ( _debug->isDebug( ) ) {
		_debug->addLog( "監視時間:" + std::to_string( _looking_time / FPS ) + "秒" );
	}
}

void CharaDummy::walk( ) {
	if ( _dir == MOVE_DOWN ) {
		return;
	}

	//立ち止まる処理
	_wait_count++;
	if ( _wait_count >= CHECK_WAIT_TIME ) {
		if ( _random->getRealOne() <= WAIT_PROBABILITY ) {
			setAnim( Character::ANIM_WAIT );
			_wait_time = _random->getInt32( 2, 4 );
		}
		_wait_count = 0;
	}

	// 進行方向に予測座標をセット
	setDistance( );

	checkCollider( );
}

void CharaDummy::wait( ) {
	_wait_ani_time++;

	//2~4秒経ったらWalkに戻る
	if ( _wait_ani_time >= _wait_time * FPS ) {
		setAnim( Character::ANIM_WALK );
		_wait_ani_time = 0;
	}
}
void CharaDummy::fall( ) {
	MOVE_DIRECTION past = _dir;
	_dir = MOVE_DOWN;

	// 進行方向に予測座標をセット
	setDistance( );

	// 横方向に進行方向を戻す
	_dir = past;

	bool move_ok = true;
	
	int data = getMapData( getPos( ) + _distance );
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
	int data = getMapData( getPos( ) + _distance );

	// エレベーターだったら
	if ( data < 0 ) {
		return;
	}

	// 進行予想値がシャッター
	if ( data == IDENTIFICATION_SHUTTER ) {
		_return_move = true;
	}

	// 進行予測値が当たり判定であったら
	if ( data == IDENTIFICATION_COLLIDER ) {
		// もう1つ上を見る
		data = getMapData( getPos( ) + _distance + Vector( 0, -BLOCK_SIZE ) );
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
	char id = getElevatorId( getPos( ) );

	// エレベーター以外だったら
	if ( id == 0x00 ) {
		_ride_elevator = false;
		return;
	}

	// 連続で乗るのを防ぐ
	if ( _ride_elevator ) {
		return;
	}

	// エレベータの状態を取得
	ELEVATOR_STATE state = getElevatorState( id, getPos( ) );

	switch ( state ) {
	case ELEVATOR_STATE_WAIT:
		if ( getAnimType( ) != Character::ANIM_WAIT_ELEVATOR ) {
			setAnim( Character::ANIM_WAIT_ELEVATOR );
		}
		return;

	case ELEVATOR_STATE_MOVE:
	{
		Character::ANIM_TYPE anim = getAnimType( );
		if ( anim != Character::ANIM_RIDE && anim == Character::ANIM_WAIT_ELEVATOR ) {
			setAnim( Character::ANIM_RIDE );
		}
		return;
	}

	case ELEVATOR_STATE_ARRIVE:
		if ( getAnimType( ) != Character::ANIM_RIDE ) {
			break;
		}
		setElevatorPos( id );
		setAnim( Character::ANIM_WALK );
		_ride_elevator = true;
		return;

	default:
		break;
	}
}

void CharaDummy::returnMove( ) {
	switch ( _dir ) {
	case MOVE_RIGHT: _dir = MOVE_LEFT ; break;
	case MOVE_LEFT : _dir = MOVE_RIGHT; break;
	default: return;
	}
}