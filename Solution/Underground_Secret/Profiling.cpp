#include "Profiling.h"
#include "Mouse.h"
#include "Drawer.h"
#include "const.h"
#include "sound.h"
#include "button.h"

const int ACTIVE_BUTTON_X = 1024;
const int ACTIVE_BUTTON_Y = 0;
const float INITIAL_VELOCITY = -23; // 初速度
const float ACCELERATION = 0.4f;
const float TOTAL_TIME = 1.1f; //ボードが出現から止まるまでの時間（秒）
const float NONACTIVE_BOARD_Y = HEIGHT;
const int BOARD_EXIT_SPEED = 50;
const char PROFILING_BUTTON[ ] = "ProfilingOpen";
const char PROFILING_BUTTON_PUSH[ ] = "ProfilingOpen_push";
const char PROFILING_BACK_BUTTON[ ] = "ProfilingClose";
const char PROFILING_BACK_BUTTON_PUSH[ ] = "ProfilingClose_push";

Profiling::Profiling( std::vector< std::string > &profiling ) :
_profiling( profiling ),
_board_count( 0 ),
_board_y( HEIGHT_F ) {
	_mouse = Mouse::getTask( );
	_sound = Sound::getTask( );
	_drawer = Drawer::getTask( );

	_active_button_width = _drawer->getImageWidth( PROFILING_BUTTON );
	_active_button_height = _drawer->getImageHeight( PROFILING_BUTTON );
	_board_handle = _drawer->getImage( "ProfilingBoard" );
	_board_width = _drawer->getImageWidth( "ProfilingBoard" );
	_board_height = _drawer->getImageHeight( "ProfilingBoard" );

	//SE
	_sound_handle[ BOARD_MOVE ] = _sound->load( "/SoundEffect/BoardMove.ogg" );

	//ボタン
	Vector button_pos = Vector( WIDTH - _active_button_width / 2.0f, _active_button_height / 2.0f );
	_button = ButtonPtr( new Button(
		( float )button_pos.x - _active_button_width / 2.0f,
		( float )button_pos.y - _active_button_height / 2.0f,
		( float )button_pos.x + _active_button_width / 2.0f,
		( float )button_pos.y + _active_button_height / 2.0f ) );
	_button->setPos( ( float )button_pos.x, ( float )button_pos.y );
}

Profiling::~Profiling( ) {
}

void Profiling::update( ) {
	calcActiveButton( );
	calcBoardPos( );
}

void Profiling::draw( ) const {
	drawActiveButton( );
	drawBoard( );
	drawProfiling( );
}

void Profiling::drawActiveButton( ) const {
	if ( _active ) {
		_button->setImage( PROFILING_BACK_BUTTON );
		_button->setPushImage( PROFILING_BACK_BUTTON_PUSH );
	} else {
		_button->setImage( PROFILING_BUTTON );
		_button->setPushImage( PROFILING_BUTTON_PUSH );
	}

	_button->draw( false );
}

void Profiling::drawBoard( ) const {
	const int BOARD_WIDTH = 700;
	const int BOARD_HEIGHT = 1000;
	const float BOARD_X = ( WIDTH_F - BOARD_WIDTH ) / 2;
	_drawer->drawExtendGraph( BOARD_X, _board_y, BOARD_X + BOARD_WIDTH, _board_y + BOARD_HEIGHT, _board_handle, true );
}

void Profiling::drawProfiling( ) const {
	int size = ( int )_profiling.size( );
	const float PROFILING_X = WIDTH / 2;
	const float PITCH = ( float )( _board_height / ( size + 2 ) );
	const float ADJUST_Y = 100;

	for ( int i = 0; i < size; i++ ) {
		// 余白 + i * ピッチ
		float y = PITCH + _board_y + PITCH * i + ADJUST_Y;
		_drawer->drawFormatStringCenter( PROFILING_X, y, BLUE, _profiling[ i ].c_str( ) );
	}
}

void Profiling::calcActiveButton( ) {
	
	//ボードが出現、退出の途中にボタンを無効化にする
	if ( _board_y != HEIGHT && !_active ) {
		return;
	}
	if ( _board_count != FPS * TOTAL_TIME && _active ) {
		return;
	}

	Vector mouse_pos = _mouse->getPoint( );
	if ( _mouse->getClickingLeft( ) ) {
		_button->click( mouse_pos );
	} else {
		if ( _button->isPush( ) ) {
			_sound->play( _sound_handle[ BOARD_MOVE ] );
			_active = !_active;
		}
		_button->resetState( );
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
		_board_count = ( int )( FPS * TOTAL_TIME );
	}

	_board_y = ( float )( INITIAL_VELOCITY * _board_count + 0.5 * ACCELERATION * _board_count * _board_count + HEIGHT ); // x = V0t + at^2 / 2 + 初期地点
}

bool Profiling::isActive( ) const {
	return _active;
}