#include "Button.h"
#include "Drawer.h"

Button::Button( float lx, float ly, float rx, float ry ) :
_lx( lx ),
_ly( ly ),
_rx( rx ),
_ry( ry ),
_x ( 0 ),
_y ( 0 ),
_x2( -1 ),
_y2( -1 ),
_state( Button::BUTTON_STATE_NORMAL ) {
	_drawer = Drawer::getTask( );
}

Button::~Button( ) {
}

void Button::draw( ) const {
	int handle = -1;
	if ( _state == Button::BUTTON_STATE_NORMAL ) {
		handle = _handle;
	}
	if ( _state == Button::BUTTON_STATE_PUSH ) {
		handle = _push_handle;
	}

	if ( _x2 == -1.0f || _y2 == -1.0f ) {
		_drawer->drawRotaGraph( _x, _y, 1, 0, handle, true );
	} else {
		_drawer->drawExtendGraph( _x, _y, _x2, _y2, handle, true );
	}

	// debug
	//_drawer->drawCircle( _lx, _ly, 30, 0xff0000, true );
	//_drawer->drawCircle( _lx, _ry, 30, 0xff0000, true );
	//_drawer->drawCircle( _rx, _ly, 30, 0xff0000, true );
	//_drawer->drawCircle( _rx, _ry, 30, 0xff0000, true );
}

void Button::setCollider( float lx, float ly, float rx, float ry ) {
	_lx = lx;
	_ly = ly;
	_rx = rx;
	_ry = ry;
}

void Button::setPos( float x, float y, float x2, float y2 ) {
	x = x;
	y = y;
	if ( x2 != -1 && y2 != -1 ) {
		_x2 = x2;
		_y2 = y2;
	}
}

void Button::setImage( const char* filepath ) {
	_handle = _drawer->getImage( filepath );
}

void Button::setPushImage( const char* filepath ) {
	_push_handle = _drawer->getImage( filepath );
}