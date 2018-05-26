#include "Profiling.h"
#include "Mouse.h"
#include "Drawer.h"
#include "const.h"

const int ACTIVE_BUTTON_X = ( WIDTH / 5 ) * 4;
const int ACTIVE_BUTTON_Y = 0;
const float INITIAL_VELOCITY = -22; // 初速度
const float ACCELERATION = 0.4f;
const float TOTAL_TIME = 1.1f; //ボードが出現から止まるまでの時間（秒）
const float NONACTIVE_BOARD_Y = HEIGHT;

Profiling::Profiling( ) {
	_mouse = Mouse::getTask( );
	_drawer = Drawer::getTask( );
	_active_button_handle = _drawer->getImage( "Profiling" );
	_active_button_width = _drawer->getImageWidth( "Profiling" );
	_active_button_height = _drawer->getImageHeight( "Profiling" );
	_back_button = _drawer->getImage( "back" );
	_board_handle = _drawer->getImage( "ProfilingBoard" );
	_board_y = HEIGHT_F;
	_board_count = 0;
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

	_board_count++;

	if ( _board_count >= FPS * TOTAL_TIME ) {
		_board_count = FPS * TOTAL_TIME;
	}

	_board_y = INITIAL_VELOCITY * _board_count + 0.5 * ACCELERATION * _board_count * _board_count + HEIGHT; // x = V0t + at^2 / 2 + 初期地点
}

bool Profiling::isActive( ) const {
	return _active;
}