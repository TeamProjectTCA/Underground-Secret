#include "CharaDummy.h"
#include "const.h"

const int MOVE_RATE_X = 3;
const int MOVE_RATE_Y = BLOCK_SIZE / 2;

CharaDummy::CharaDummy( MapPtr map ) :
Character( map ) {
	addAnim( Character::WALK, "CharaDummy_Walk", 2 );
	addAnim( Character::OPEN, "CharaDummy_Open", 2 );
	setAnim( Character::WALK );

	_dir = MOVE_RIGHT;
	setDistance( );
}

CharaDummy::~CharaDummy( ) {
}

void CharaDummy::update( ) {
	_dir = MOVE_RIGHT;
	walk( );
	fall( );
}

void CharaDummy::walk( ) {
	if ( _dir == MOVE_DOWN ) {
		return;
	}

	setDistance( );
	bool move_ok = true;

	int data = getMapData( getPos( ) + _distance );

	// 変なところをみていたら
	if ( data < 0 ) {
		return;
	}

	// 当たり判定が何かしらあれば
	if ( data != IDENTIFICATION_NONE ) {
		switch ( data ) {
		// 当たり判定
		case IDENTIFICATION_COLLIDER:
			{
				data = getMapData( getPos( ) + _distance + Vector( 0, -BLOCK_SIZE ) );
				if ( data != IDENTIFICATION_COLLIDER ) {
					_distance += Vector( 0, -BLOCK_SIZE );
					break;
				}
				move_ok = false;
			}
			break;

		// シャッター
		case IDENTIFICATION_SHUTTER:

			move_ok = false;
			break;

		default:
			break;
		}
	}

	if ( !move_ok ) {
		return;
	}

	// 移動させる	
	move( _distance );
}

void CharaDummy::fall( ) {
	_dir = MOVE_DOWN;

	setDistance( );
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
	move( _distance );
}

void CharaDummy::setDistance( ) {
	switch ( _dir ) {
	case MOVE_RIGHT: _distance = Vector(  MOVE_RATE_X, 0 ); break;
	case MOVE_LEFT : _distance = Vector( -MOVE_RATE_X, 0 ); break;
	case MOVE_DOWN : _distance = Vector(  0, MOVE_RATE_Y ); break;
	}
}