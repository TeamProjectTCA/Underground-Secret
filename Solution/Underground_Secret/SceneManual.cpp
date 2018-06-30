#include "SceneManual.h"
#include "Keyboard.h"
#include "Drawer.h"
#include "const.h"
#include "Button.h"
#include "Vector.h"
#include "Mouse.h"
#include "Sound.h"

const char MANUAL_STRING_IMAGE     [ ] = "manual";
const char BACK_BUTTON_IMAGE       [ ] = "BackButton";
const char BACK_BUTTONCLICK_IMAGE  [ ] = "BackButtonClick";
const float BACK_BUTTON_SIZE = 64;

const float BOARD_LX = 240;
const float BOARD_LY = 60;
const float BOARD_RX = 1040;
const float BOARD_RY = 660;
const float BOARD_PITCH_X = 60;
const float BOARD_PITCH_UP   = 120;
const float BOARD_PITCH_DOWN = 10;

SceneManual::SceneManual( ) {
	_drawer      = Drawer::getTask( );
	_keyboard    = Keyboard::getTask( );
	_mouse       = Mouse::getTask( );
	_button      = ButtonPtr( new Button( ) );
	_back_button = ButtonPtr( new Button( ) );

	initialize( );
}

SceneManual::~SceneManual( ) {
}

void SceneManual::initialize( ) {
	_back_button->setImage( BACK_BUTTON_IMAGE );
	_back_button->setPushImage( BACK_BUTTONCLICK_IMAGE );
	_back_button->setPos( 
		BOARD_RX - BACK_BUTTON_SIZE * 2, BOARD_LY + BACK_BUTTON_SIZE * 1,
		BOARD_RX - BACK_BUTTON_SIZE * 1, BOARD_LY + BACK_BUTTON_SIZE * 2 );
	_back_button->setCollider( 
		BOARD_RX - BACK_BUTTON_SIZE * 2, BOARD_LY + BACK_BUTTON_SIZE * 1,
		BOARD_RX - BACK_BUTTON_SIZE * 1, BOARD_LY + BACK_BUTTON_SIZE * 2 );
}

void SceneManual::update( ) {
	if ( _mouse->getClickingLeft( ) ) {
		_back_button->click( _mouse->getPoint( ) );
	} else {
		if ( _back_button->isPush( ) ) {
			SoundPtr sound = Sound::getTask( );
			sound->play( sound->load( "SoundEffect/button.ogg" ), false, true );
			setNextScene( SCENE_TITLE );
		}
		_back_button->resetState( );
	}

	draw( );
}

void SceneManual::draw( ) const {
	// ”wŒi
	_drawer->drawExtendGraph( 0, 0, WIDTH_F, HEIGHT_F, _drawer->getImage( "underground_back" ), true ); //”wŒi
	_drawer->drawExtendGraph( BOARD_LX, BOARD_LY, BOARD_RX, BOARD_RY, _drawer->getImage( "board" ), true ); 
	_drawer->drawExtendGraph( 
		BOARD_LX + BOARD_PITCH_X, BOARD_LY + BOARD_PITCH_UP,
		BOARD_RX - BOARD_PITCH_X, BOARD_RY - BOARD_PITCH_DOWN, _drawer->getImage( MANUAL_STRING_IMAGE ), true );
	_back_button->draw( false );

	_drawer->flip( );
}