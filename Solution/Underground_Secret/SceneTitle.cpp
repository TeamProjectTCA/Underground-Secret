#include "SceneTitle.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "const.h"

const float START_BUTTON_X = WIDTH_F / 2.0f ;
const float START_BUTTON_Y = HEIGHT / 5 * 4 ;

SceneTitle::SceneTitle( ) {
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_mouse = Mouse::getTask( );
	_background_handle = _drawer->getImage( "back_001" );
	_title_handle = _drawer->getImage( "title" );

	_cur_hand = LoadCursor( NULL, IDC_HAND );
	std::array< int, BUTTON_STATE_MAX >( ).swap( _startbutton_handle );
	_startbutton = BoxObject( );

	_startbutton_handle[ NORMAL ] = _drawer->getImage( "GameStart" );
	_startbutton_handle[ CLICKING ] = _drawer->getImage( "GameStartClick" );
	_startbutton.image.width = _drawer->getImageWidth( "GameStart" );
	_startbutton.image.height = _drawer->getImageHeight( "GameStart" );
	_startbutton.collider.left = ( float )START_BUTTON_X - _startbutton.image.width * 0.5f;
	_startbutton.collider.right = ( float )START_BUTTON_X + _startbutton.image.width * 0.5f;
	_startbutton.collider.up = ( float )START_BUTTON_Y - _startbutton.image.height * 0.5f;
	_startbutton.collider.down = ( float )START_BUTTON_Y + _startbutton.image.height * 0.5f;
}

SceneTitle::~SceneTitle( ) {
}

void SceneTitle::initialize( ) {
	_startbutton_clicking = false;
}

void SceneTitle::finalize( ) {
}

void SceneTitle::update( ) {
	_drawer->drawString( 10, 10, "SceneTitle", _drawer->getColor( 255, 0, 0 ) );
	changeNextScene( );
	calcButtonAction( );

	drawBackGround( );
	drawTitle( );
	drawStartButton( );
}

void SceneTitle::changeNextScene( ) {
	if ( ( _startbutton_clicking ) && !isDrag( ) ) {
		int hit = getHitButton( );

		switch ( hit ) {
			case START_BUTTON:
				setNextScene( STAGESELECT );
				break;
			default:
				_startbutton_clicking = false;
				break;
		}
	}
}

void SceneTitle::calcButtonAction( ) {
	_startbutton.image.handle = _startbutton_handle[ NORMAL ];

	if ( getHitButton( ) == NONE_BUTTON ) {
		return;
	}

	if ( !isDrag( ) ) {
		return;
	}

	BUTTON hit = getHitButton( );
	switch ( hit ) {
		case START_BUTTON:
			_startbutton.image.handle = _startbutton_handle[ CLICKING ];
			_startbutton_clicking = true;
			break;
	}
}

SceneTitle::BUTTON SceneTitle::getHitButton( ) const {

	double mouse_x = _mouse->getPointX( );
	double mouse_y = _mouse->getPointY( );

	if ( _startbutton.collider.left <= mouse_x && mouse_x <= _startbutton.collider.right &&
		_startbutton.collider.up <= mouse_y && mouse_y <= _startbutton.collider.down ) {
		SetCursor( _cur_hand );
		return START_BUTTON;
	}
	return NONE_BUTTON;
}
bool SceneTitle::isDrag( ) const {
	if ( _mouse->getClickingLeft( ) > 0 ) {
		return true;
	}
	return false;
}

void SceneTitle::drawBackGround( ) const{
	_drawer->drawRotaGraph( WIDTH / 2, HEIGHT / 2, 1, 0, _background_handle, true );
}

void SceneTitle::drawTitle( ) const {
	_drawer->drawRotaGraph( WIDTH / 2, HEIGHT / 3, 1, 0, _title_handle, true );
}

void SceneTitle::drawStartButton( ) const {
	_drawer->drawRotaGraph( START_BUTTON_X, START_BUTTON_Y, 1, 0, _startbutton.image.handle, true );
}