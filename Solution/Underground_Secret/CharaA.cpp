#include "CharaA.h"
#include "Drawer.h"
#include "Keyboard.h"

const int START_CHARA_POS_X = 320;
const int START_CHARA_POS_Y = 320;

CharaA::CharaA( ) {
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_chara_handle = _drawer->getImage( "charaA" );
	_chara_x = START_CHARA_POS_X;
	_chara_y = START_CHARA_POS_Y;
	_vec_x = 0;
	_vec_y = 0;
	_enable_fall = true;
	_dir = DIR_RIGHT;
}


CharaA::~CharaA( ) {

}

void CharaA::update( ) {
	fall( );
	drawCharacter( );
	//デバッグ用
	move( );
}

void CharaA::fall( ) {
	if ( !_enable_fall ) {
		_vec_y = 0;
	} else {
		_vec_y = FALL_SPEED;
	}
	_chara_y += _vec_y;
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
	_chara_x += _vec_x;
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

void CharaA::drawCharacter( ) const {
	_drawer->drawGraph( _chara_x, _chara_y, _chara_handle, true );
}

int CharaA::getPosX( ) const {
	return _chara_x;
}

int CharaA::getPosY( ) const {
	return _chara_y;
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