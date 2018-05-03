#include "CharaA.h"
#include "Drawer.h"

CharaA::CharaA( ) {
	_drawer = Drawer::getTask( );

	_chara_handle = _drawer->getImage( "charaA" );
	_chara_x = 230;
	_chara_y = 520;
}


CharaA::~CharaA( ) {

}

void CharaA::update( ) {
	drawCharacter( );
}

void CharaA::drawCharacter( ) {
	_drawer->drawGraph( _chara_x, _chara_y, _chara_handle, true );
}