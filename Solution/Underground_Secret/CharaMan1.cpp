#include "CharaMan1.h"
#include "Drawer.h"
#include "const.h"
#include "Debug.h"
#include "Infomation.h"
#include "Random.h"

const int MOVE_RATE_X = 5;
const int MOVE_RATE_Y = BLOCK_SIZE;
const int INFO_SHOWTIME = FPS * 10;
const int CHECK_WAIT_TIME = FPS * 3;
const float WAIT_PROBABILITY = 0.1f;
const float RIDE_PROBABILITY = 0.3f;
const float DOWN_SCALE = 0.5f;
const float HIT_SHUTTER_SCALE = 1.0f;
const int WAIT_TIME = 1;

CharaMan1::CharaMan1( MapPtr map, InfomationPtr info ) :
	Character( map, info->getInfo( CHARA_MAN1 ) ),
	_ride_elevator( false ),
	_ride_probability( RIDE_PROBABILITY ),
	_return_move( false ),
	_hit_shutter( false ),
	_elevator_down( false ),
	_judged_probability( false ),
	_looking_time( 0 ),
	_wait_count( 0 ),
	_wait_ani_time( 0 ) {
	_random = Random::getTask( );

	addAnim( Character::ANIM_WALK, "man1_walk", 14 );
	addAnim( Character::ANIM_RIDE, "man1_wait", 10 );
	addAnim( Character::ANIM_WAIT, "man1_wait", 10 );
	addAnim( Character::ANIM_WAIT_ELEVATOR, "man1_wait", 10 );
	setAnim( Character::ANIM_WALK );

	_dir = ( _random->getInt32( 0, 1 ) ? MOVE_RIGHT : MOVE_LEFT );

	setDistance( );
}

CharaMan1::~CharaMan1( ) {
}

void CharaMan1::update( ) {
	setScroll( );

	if ( _move_flag ) {
		if ( getAnimType( ) == Character::ANIM_WALK ) {
			walk( );
			fall( );
			checkCaughtCollider( );
		}

		if ( getAnimType( ) == Character::ANIM_WAIT ) {
			//立ち止まる
			wait( );
		}
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

void CharaMan1::walk( ) {
	if ( _dir == MOVE_DOWN ) {
		return;
	}

	//立ち止まる処理
	_wait_count++;
	if ( _wait_count >= CHECK_WAIT_TIME ) {
		if ( _random->getRealOne( ) <= WAIT_PROBABILITY ) {
			setAnim( Character::ANIM_WAIT );
		}
		_wait_count = 0;
	}

	// 進行方向に予測座標をセット
	setDistance( );

	checkCollider( );
}

void CharaMan1::wait( ) {
	_wait_ani_time++;

	//2~4秒経ったらWalkに戻る
	if ( _wait_ani_time >= WAIT_TIME * FPS ) {
		setAnim( Character::ANIM_WALK );
		_wait_ani_time = 0;
	}
}
void CharaMan1::fall( ) {
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

void CharaMan1::setDistance( ) {
	switch ( _dir ) {
		case MOVE_RIGHT: _distance = Vector( MOVE_RATE_X, 0 ); break;
		case MOVE_LEFT: _distance = Vector( -MOVE_RATE_X, 0 ); break;
		case MOVE_DOWN: _distance = Vector( 0, MOVE_RATE_Y ); break;
	}
}

void CharaMan1::countLooking( ) {
	if ( isLooking( getPos( ) ) ) {
		_looking_time++;
	}
}

void CharaMan1::checkCollider( ) {
	// アスキーコードを取得
	int data = getMapData( getPos( ) + _distance );

	// エレベーターだったら
	if ( data < 0 ) {
		return;
	}

	// 進行予想値がシャッター
	if ( data == IDENTIFICATION_SHUTTER ) {
		// 自分のいる位置もシャッターであれば進行する(埋まるのを回避)
		if ( getMapData( getPos( ) ) != IDENTIFICATION_SHUTTER ) {
			_return_move = true;
			_hit_shutter = true;
		}
	}

	// 進行予測値が当たり判定であったら
	if ( data == IDENTIFICATION_COLLIDER ) {
		// もう1つ上を見る
		data = getMapData( getPos( ) + _distance + Vector( 0, -BLOCK_SIZE ) );
		if ( data == IDENTIFICATION_COLLIDER ) {
			// 2回目も当たり判定があったら
			_return_move = true;
			_hit_shutter = false;
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

void CharaMan1::checkElevator( ) {
	// アスキーコードを取得
	char id = getElevatorId( );

	// エレベーター以外だったら
	if ( id == 0x00 ) {
		_ride_elevator = false;
		_judged_probability = false;
		return;
	}

	// 連続で乗る・判定するのを防ぐ
	if ( _ride_elevator ) {
		return;
	}

	// エレベータの状態を取得
	ELEVATOR_STATE state = getElevatorState( );
	ELEVATOR_POS active_elevator = getActiveElevator( );
	ELEVATOR_POS destination = getDestination( );
	ELEVATOR_POS elevator_pos = getElevatorPos( );
	_draw_flag = true;

	switch ( state ) {
		case ELEVATOR_STATE_NONE:
			break;

		case ELEVATOR_STATE_COME:
		{
			Character::ANIM_TYPE anim = getAnimType( );
			if ( !isActiveElevatorPos( ) ) {
				break;
			}

			if ( anim != Character::ANIM_RIDE && anim == Character::ANIM_WAIT_ELEVATOR ) {
				setAnim( Character::ANIM_RIDE );
			}
			break;
		}

		case ELEVATOR_STATE_MOVE:
		{
			Character::ANIM_TYPE anim = getAnimType( );
			if ( anim == Character::ANIM_RIDE ) {
				_draw_flag = false;
			}
			break;
		}

		case ELEVATOR_STATE_ARRIVE:
			if ( getAnimType( ) != Character::ANIM_RIDE ) {
				break;
			}
			setElevatorPos( id );
			setAnim( Character::ANIM_WALK );
			_ride_elevator = true;
			break;

		default:
			if ( getAnimType( ) != Character::ANIM_WAIT_ELEVATOR ) {
				// 連続で判定するのを防ぐ
				if ( _judged_probability ) {
					return;
				}

				if ( elevator_pos == ELEVATOR_POS_UP ||
				   ( elevator_pos == ELEVATOR_POS_CENTER && destination == ELEVATOR_POS_DOWN ) ) {
					_elevator_down = true;
				} else {
					_elevator_down = false;
				}

				//エレベーターに乗る確率判定
				_ride_probability = RIDE_PROBABILITY;
				if ( _hit_shutter ) {
					_ride_probability += RIDE_PROBABILITY * HIT_SHUTTER_SCALE;
				}

				if ( isSpy( ) && _elevator_down ) {
					_ride_probability += RIDE_PROBABILITY * DOWN_SCALE;
				}

				_judged_probability = true;
				if ( _random->getRealOne( ) > _ride_probability ) {
					return;
				}

				setAnim( Character::ANIM_WAIT_ELEVATOR );
			}
			break;
	}
}

void CharaMan1::returnMove( ) {
	switch ( _dir ) {
		case MOVE_RIGHT: _dir = MOVE_LEFT; break;
		case MOVE_LEFT: _dir = MOVE_RIGHT; break;
		default: return;
	}
}