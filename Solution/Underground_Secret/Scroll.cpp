#include "Scroll.h"
#include "Keyboard.h"
#include "const.h"

const int SCROLL_SIZE = 2;

Scroll::Scroll( ) {
	_keyboard = Keyboard::getTask( );
	_scroll = Vector( );
	_col = 0;
	_row = 0;
}

Scroll::~Scroll( ) {
}

void Scroll::update( ) {
	//¶
	if ( _keyboard->getKeyDown( "d" ) || _keyboard->getState( "d" ) > 30 ) {
		_scroll.x -= SCROLL_SIZE;
	}
	//‰E
	if ( _keyboard->getKeyDown( "a" ) || _keyboard->getState( "a" ) > 30 ) {
		_scroll.x += SCROLL_SIZE;
	}
	//ã
	if ( _keyboard->getKeyDown( "s" ) || _keyboard->getState( "s" ) > 30 ) {
		_scroll.y -= SCROLL_SIZE;
	}
	//‰º
	if ( _keyboard->getKeyDown( "w" ) || _keyboard->getState( "w" ) > 30 ) {
		_scroll.y += SCROLL_SIZE;
	}

	if ( _scroll.x > 0 ) {
		_scroll.x = 0;
	}
	if ( _scroll.x < ( WIDTH / BLOCK_SIZE - ( _col ) ) ) {
		_scroll.x = WIDTH / BLOCK_SIZE - ( _col );
	}

	if ( _scroll.y > 0 ) {
		_scroll.y = 0;
	}
	if ( _scroll.y < HEIGHT / BLOCK_SIZE - ( _row ) ) {
		_scroll.y = HEIGHT / BLOCK_SIZE - ( _row );
	}
}

void Scroll::setCol( const int col ) {
	_col = col;
}

void Scroll::setRow( const int row ) {
	_row = row;
}

Vector Scroll::getScroll( ) const {
	return _scroll;
}