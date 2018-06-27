#include "Scroll.h"
#include "Keyboard.h"
#include "const.h"

const int SCROLL_SPEED = 5;

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
		_scroll.x -= SCROLL_SPEED;
	}
	//‰E
	if ( _keyboard->getKeyDown( "a" ) || _keyboard->getState( "a" ) > 30 ) {
		_scroll.x += SCROLL_SPEED;
	}
	//ã
	if ( _keyboard->getKeyDown( "s" ) || _keyboard->getState( "s" ) > 30 ) {
		_scroll.y -= SCROLL_SPEED;
	}
	//‰º
	if ( _keyboard->getKeyDown( "w" ) || _keyboard->getState( "w" ) > 30 ) {
		_scroll.y += SCROLL_SPEED;
	}

	if ( _scroll.x > 0 ) {
		_scroll.x = 0;
	}
	if ( _scroll.x < WIDTH - _col * BLOCK_SIZE ) {
		_scroll.x = WIDTH - _col * BLOCK_SIZE;
	}

	if ( _scroll.y > 0 ) {
		_scroll.y = 0;
	}
	if ( _scroll.y < HEIGHT - _row * BLOCK_SIZE ) {
		_scroll.y = HEIGHT - _row * BLOCK_SIZE;
	}
}

void Scroll::setCol( const int col ) {
	_col = col;
}

void Scroll::setRow( const int row ) {
	_row = row;
}

void Scroll::setScroll( Vector vec ) {
	_scroll += vec;
}

Vector Scroll::getScroll( ) const {
	return _scroll;
}