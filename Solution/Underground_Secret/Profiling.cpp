#include "Profiling.h"
#include "Mouse.h"
#include "Drawer.h"
#include "const.h"

const int ACTIVE_BUTTON_X = ( WIDTH / 5 ) * 4;
const int ACTIVE_BUTTON_Y = 0;
const float UP_LENGTH = ( HEIGHT_F / 6 * 5 );
const float DOWN_LENGTH = UP_LENGTH - ( HEIGHT_F / 5 * 4 );
const float INITIAL_VELOCITY = 50; // ‰‘¬“x
const float ACCELERATION = ( INITIAL_VELOCITY * INITIAL_VELOCITY ) / UP_LENGTH;
const float UP_TIME = INITIAL_VELOCITY / ACCELERATION; //t1
const float DOWN_TIME = ( 2 * DOWN_LENGTH ) / ACCELERATION; //t2‚Ì2æ
const float NONACTIVE_BOARD_Y = HEIGHT;
const float DECELERATE_POS = HEIGHT / 2;

Profiling::Profiling( ) {
	_mouse = Mouse::getTask( );
	_drawer = Drawer::getTask( );
	_active_button_handle = _drawer->getImage( "Profiling" );
	_active_button_width = _drawer->getImageWidth( "Profiling" );
	_active_button_height = _drawer->getImageHeight( "Profiling" );
	_back_button = _drawer->getImage( "back" );
	_board_handle = _drawer->getImage( "ProfilingBoard" );
	_board_y = HEIGHT_F;
	_board_up_count = 0;
	_board_down_count = 0;
}

Profiling::~Profiling( ) {
}

void Profiling::update( ) {
	calcActiveButton( );
	calcBoardPos( );

	draw( );
}

void Profiling::draw( ) {
	drawActiveButton( );
	drawBoard( );
}

void Profiling::drawActiveButton( ) const {
	int handle = _active_button_handle;
	if ( _active ) {
		handle = _back_button;
	}

	_drawer->drawGraph( ACTIVE_BUTTON_X, ACTIVE_BUTTON_Y, handle, true );
}

void Profiling::drawBoard( ) const {
	_drawer->drawGraph( WIDTH_F / 4, _board_y, _board_handle, true );

	_drawer->drawFormatString( 20, 100, RED, "%.lf", _board_y );
}

void Profiling::calcActiveButton( ) {
	if ( !_mouse->isClickDownLeft( ) ) {
		return;
	}

	double mouse_x = ( double )_mouse->getPointX( );
	double mouse_y = ( double )_mouse->getPointY( );

	if ( ACTIVE_BUTTON_X <= mouse_x && mouse_x <= ACTIVE_BUTTON_X + _active_button_width &&
		 ACTIVE_BUTTON_Y <= mouse_y && mouse_y <= ACTIVE_BUTTON_Y + _active_button_height ) {
		_active = !_active;
	}
}

void Profiling::calcBoardPos( ) {
	if ( !_active ) {
		return;
	}

	bool up = true;

	// ã¸‚µ‚«‚Á‚½‚©‚Ç‚¤‚©
	//if ( UP_TIME <= _board_up_count ) {
	//	up = false;
	//}

	// ã¸‚©‰º~
	if ( up ) {
		_board_up_count++;
		float t = ( float )( _board_up_count / FPS );

		_board_y = -( INITIAL_VELOCITY * t - ( ACCELERATION * ( t * t ) ) / 2 ) + HEIGHT; // x = V0t + at^2 / 2 + ‰Šú’n“_

	} else {
		_board_down_count++;
	}
}

bool Profiling::isActive( ) const {
	return _active;
}