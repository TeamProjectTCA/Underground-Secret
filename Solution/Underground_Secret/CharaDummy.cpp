#include "CharaDummy.h"

CharaDummy::CharaDummy( MapPtr map ) :
Character( map ) {
	addAnim( Character::WALK, "CharaDummy_Walk", 2 );
	addAnim( Character::OPEN, "CharaDummy_Open", 2 );
	setAnim( Character::WALK );
}

CharaDummy::~CharaDummy( ) {
}

void CharaDummy::update( ) {
	_pos.x++;
}
