#include "Button.h"
#include "Drawer.h"

Button::Button( float lx, float ly, float rx, float ry ) :
_lx( lx ),
_ly( ly ),
_rx( rx ),
_ry( ry ),
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

	_drawer->drawRotaGraph( _rx - ( _rx - _lx ) / 2, _ry - ( _ry - _ly ) / 2, 1, 0, handle, true );

	// debug
	//_drawer->drawCircle( _lx, _ly, 30, 0xff0000, true );
	//_drawer->drawCircle( _lx, _ry, 30, 0xff0000, true );
	//_drawer->drawCircle( _rx, _ly, 30, 0xff0000, true );
	//_drawer->drawCircle( _rx, _ry, 30, 0xff0000, true );
}

void Button::setPos( float lx, float ly, float rx, float ry ) {
	_lx = lx;
	_ly = ly;
	_rx = rx;
	_ry = ry;
}

void Button::setImage( const char* filepath ) {
	_handle = _drawer->getImage( filepath );
}

void Button::setPushImage( const char* filepath ) {
	_push_handle = _drawer->getImage( filepath );
}