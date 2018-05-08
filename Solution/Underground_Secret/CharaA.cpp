#include "CharaA.h"
#include "Drawer.h"
#include "Keyboard.h"

const int START_CHARA_POS_X = 320;
const int START_CHARA_POS_Y = 320;

CharaA::CharaA( MapPtr map ) :
Character( map ) {
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );

	addAnim( Character::WALK, "CharaA_Walk", 1 );
	setAnim( Character::WALK );

	_pos = Vector( START_CHARA_POS_X, START_CHARA_POS_Y );
	_vec_x = 0;
	_vec_y = 0;
	_enable_fall = true;
	_dir = DIR_RIGHT;
}


CharaA::~CharaA( ) {

}

void CharaA::update( ) {
	_pos.x++;
	//fall( );
	////デバッグ用
	//move( );
}

void CharaA::fall( ) {
	if ( !_enable_fall ) {
		_vec_y = 0;
	} else {
		_vec_y = FALL_SPEED;
	}
	_pos.y += _vec_y;
}

void CharaA::move( ) {
	_vec_x = 0;
	if ( _keyboard->getState( "RIGHT_ARROW" ) ) {
		if ( !_enable_move ) {
			_vec_x = 0;
		} else {
			_vec_x = 2;
		}
		_dir = DIR_RIGHT;
	}
	if ( _keyboard->getState( "LEFT_ARROW" ) ) {
		if ( !_enable_move ) {
			_vec_x = 0;
		} else {
			_vec_x = -2;
		}
		_dir = DIR_LEFT;
	}
	_pos.x += _vec_x;
}

void CharaA::setFall( bool fall ) {
	_enable_fall = fall;
}

void CharaA::setMove( bool move ) {
	_enable_move = move;
}

void CharaA::setMoveUp( bool moveup ) {
	_enable_move_up = moveup;
}

int CharaA::getWidth( ) const {
	return _drawer->getImageWidth( "charaA" );
}

int CharaA::getHeight( ) const {
	return _drawer->getImageHeight( "charaA" );
}

DIR CharaA::getDir( ) const {
	return _dir;
}