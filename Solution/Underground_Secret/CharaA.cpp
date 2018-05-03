#include "CharaA.h"
#include "Drawer.h"

const int START_CHARA_POS_X = 230;
const int START_CHARA_POS_Y = 520;

CharaA::CharaA( ) {
	_drawer = Drawer::getTask( );
	_chara_handle = _drawer->getImage( "charaA" );
	_chara_x = START_CHARA_POS_X;
	_chara_y = START_CHARA_POS_Y;
	_vec_x = 0;
	_vec_y = 0;
	_check_x = 0;
	_check_y = 0;
}


CharaA::~CharaA( ) {

}

void CharaA::update( ) {
	fall( );
	drawCharacter( );
}

void CharaA::fall( ) {
	
	_vec_y = FALL_SPEED;
	_chara_y += _vec_y;
}

void CharaA::colliderSet( ) {
	const int CHARA_WIDTH = _drawer->getImageWidth( "charaA" );
	const int CHARA_HEIGHT = _drawer->getImageHeight( "charaA" );
	if ( _dir == DIR_LEFT ) {
		_check_x = _chara_x - 1;
	}
	if ( _dir == DIR_RIGHT ) {
		_check_x = _chara_x + CHARA_WIDTH + 1;
	}
	_check_y = _chara_y + CHARA_HEIGHT + 1;
}

void CharaA::drawCharacter( ) {
	_drawer->drawGraph( _chara_x, _chara_y, _chara_handle, true );
}