#include "Profiling.h"
#include "Mouse.h"
#include "Drawer.h"
#include "const.h"
#include <string>

const int ACTIVE_BUTTON_X = ( WIDTH / 5 ) * 4;
const int ACTIVE_BUTTON_Y = 0;
const float INITIAL_VELOCITY = -22; // 初速度
const float ACCELERATION = 0.4f;
const float TOTAL_TIME = 1.1f; //ボードが出現から止まるまでの時間（秒）
const float NONACTIVE_BOARD_Y = HEIGHT;
const int BOARD_EXIT_SPEED = 50;

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
	drawHint( );
}

void Profiling::drawActiveButton( ) const {
	int handle = _active_button_handle;
	if ( _active ) {
		handle = _back_button;
	}

	_drawer->drawGraph( ACTIVE_BUTTON_X, ACTIVE_BUTTON_Y, handle, true );
}

void Profiling::drawBoard( ) const {
	int _board_width = _drawer->getImageWidth("ProfilingBoard");
	_drawer->drawGraph( ( float )( WIDTH_F - _board_width) / 2, _board_y, _board_handle, true);

	_drawer->drawFormatString( 20, 100, RED, "%.lf", _board_y );
}

void Profiling::drawHint( ) const {
	int _hint_count = 5;
	int _board_height = _drawer->getImageHeight("ProfilingBoard");
	int _hint_width = 300;
	int _hint_height = 50;
	int _hint_interval = _hint_height / 2;
	float _hint_x = ( float )( WIDTH_F - _hint_width ) / 2;
	float _hint_start_y = ( float )( _board_height - _hint_height * _hint_count - _hint_interval * ( _hint_count - 1 ) ) / 2 + _board_y;
	for ( int i = 0; i < _hint_count; i++ ) {
		float _hint_y = _hint_start_y + i * (_hint_height + _hint_interval);
		_drawer->drawBox(_hint_x, _hint_y, _hint_x + _hint_width, _hint_y + _hint_height, BLUE, true);
		_drawer->drawFormatString(_hint_x + 120, _hint_y + 15, RED, "hint %d", i + 1 );
	}
}

void Profiling::calcActiveButton( ) {
	if ( !_mouse->isClickDownLeft( ) ) {
		return;
	}
	//ボードが出現、退出の途中にボタンを無効化にする
	if (_board_y != HEIGHT && !_active) {
		return;
	}
	if (_board_count != FPS * TOTAL_TIME && _active) {
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
		_board_count = 0;
		_board_y += BOARD_EXIT_SPEED;
		if (_board_y >= HEIGHT) {
			_board_y = HEIGHT;
		}
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