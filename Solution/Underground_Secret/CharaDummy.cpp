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
	_distance = Vector( );
}

CharaDummy::~CharaDummy( ) {
}

void CharaDummy::update( ) {
	walk( );
	fall( );
}

void CharaDummy::walk( ) {
	if ( _dir == MOVE_DOWN ) {
		return;
	}

	bool move_ok = true;

	int data = getMapData( getPos( ) + _distance );

	// “–‚½‚è”»’è‚ª‰½‚©‚µ‚ç‚ ‚ê‚Î
	if ( data != IDENTIFICATION_NONE ) {
		switch ( _dir ) {
		case MOVE_RIGHT:
			{
				if ( data == IDENTIFICATION_COLLIDER ) {

				}
			}
			break;

		case MOVE_LEFT:
			break;

		default:
			break;
		}
	}

	if ( !move_ok ) {
		return;
	}

	// ˆÚ“®‚³‚¹‚é
	move( _distance );
}

void CharaDummy::fall( ) {
	if ( _dir != MOVE_DOWN ) {
		return;
	}

	bool move_ok = false;

	if ( !move_ok ) {
		return;
	}

	// ˆÚ“®‚³‚¹‚é
	move( _distance );
}

void CharaDummy::setDistance( ) {
	switch ( _dir ) {
	case MOVE_RIGHT: _distance = Vector(  MOVE_RATE_X, 0 );
	case MOVE_LEFT : _distance = Vector( -MOVE_RATE_X, 0 );
	case MOVE_DOWN : _distance = Vector(  0, MOVE_RATE_Y );
	}
}