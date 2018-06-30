#include "SceneTitle.h"
#include "Drawer.h"
#include "Sound.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Button.h"
#include "const.h"

const Vector START_BUTTON_POSITION = Vector( WIDTH / 2, HEIGHT / 5 * 3 );
const Vector MANUAL_BUTTON_POSITION = Vector( WIDTH / 2, HEIGHT / 5 * 4 );
const char START_NORMAL_IMAGE[ ] = "GameStart";
const char START_PUSH_IMAGE[ ] = "GameStartClick";
const char MANUAL_NORMAL_IMAGE[ ] = "Manual";
const char MANUAL_PUSH_IMAGE[ ] = "ManualClick";
const char BACKGROUND_IMAGE[ ] = "underground_back";
const char TITLE_IMAGE[ ] = "title";

SceneTitle::SceneTitle( ) {
	_drawer = Drawer::getTask( );
	_sound = Sound::getTask( );
	_keyboard = Keyboard::getTask( );
	_mouse = Mouse::getTask( );
	_background_image = _drawer->getImage( BACKGROUND_IMAGE );
	_title_image = _drawer->getImage( TITLE_IMAGE );

	_cur_hand = LoadCursor( NULL, IDC_HAND );

	//ƒ{ƒ^ƒ“
	int start_button_width = _drawer->getImageWidth( START_NORMAL_IMAGE );
	int start_button_height = _drawer->getImageHeight( START_NORMAL_IMAGE );
	_start_button = ButtonPtr( new Button(
		( float )START_BUTTON_POSITION.x - start_button_width / 2.0f,
		( float )START_BUTTON_POSITION.y - start_button_height / 2.0f,
		( float )START_BUTTON_POSITION.x + start_button_width / 2.0f,
		( float )START_BUTTON_POSITION.y + start_button_height / 2.0f ) );
	_start_button->setPos( ( float )START_BUTTON_POSITION.x, ( float )START_BUTTON_POSITION.y );
	_start_button->setImage( START_NORMAL_IMAGE );
	_start_button->setPushImage( START_PUSH_IMAGE ); 

	int manual_button_width = _drawer->getImageWidth( MANUAL_NORMAL_IMAGE );
	int manual_button_height = _drawer->getImageHeight( MANUAL_NORMAL_IMAGE );
	_manual_button = ButtonPtr( new Button(
		( float )MANUAL_BUTTON_POSITION.x - manual_button_width / 2.0f,
		( float )MANUAL_BUTTON_POSITION.y - manual_button_height / 2.0f,
		( float )MANUAL_BUTTON_POSITION.x + manual_button_width / 2.0f,
		( float )MANUAL_BUTTON_POSITION.y + manual_button_height / 2.0f ) );
	_manual_button->setPos( ( float )MANUAL_BUTTON_POSITION.x, ( float )MANUAL_BUTTON_POSITION.y );
	_manual_button->setImage( MANUAL_NORMAL_IMAGE );
	_manual_button->setPushImage( MANUAL_PUSH_IMAGE );

	//BGM
	_sound_handle[ TITLE_BGM ] = _sound->load( "TitleBGM/titlebgm.ogg" );
	if ( !_sound->isPlaying( _sound_handle[ TITLE_BGM ] ) ) {
		_sound->play( _sound_handle[ TITLE_BGM ], true, true );
	}

	//SE
	_sound_handle[ BUTTON_SE ] = _sound->load( "SoundEffect/button.ogg" );
}

SceneTitle::~SceneTitle( ) {
}

void SceneTitle::initialize( ) {
	_startbutton_clicking = false;
}

void SceneTitle::finalize( ) {
}

void SceneTitle::update( ) {
	Vector mouse_pos = _mouse->getPoint( );
	if ( _mouse->getClickingLeft( ) ) {
		_start_button->click( mouse_pos );
	} else {
		if ( _start_button->isPush( ) ) {
			_sound->stop( _sound_handle[ TITLE_BGM ] );
			_sound->play( _sound_handle[ BUTTON_SE ], false, true );
			setNextScene( SCENE_STAGESELECT );
		}
		_start_button->resetState( );
	}
	if ( _mouse->getClickingLeft( ) ) {
		_manual_button->click( mouse_pos );
	} else {
		if ( _manual_button->isPush( ) ) {
			_sound->play( _sound_handle[ BUTTON_SE ] );
			setNextScene( SCENE_MANUAL );
		}
		_manual_button->resetState( );
	}
	draw( );

}

void SceneTitle::draw( ) const {
	_drawer->drawExtendGraph( 0, 0, WIDTH_F, HEIGHT_F, _background_image, true ); //”wŒi
	_drawer->drawRotaGraph( WIDTH / 2, HEIGHT / 7 * 2, 1, 0, _title_image, true ); //ƒ^ƒCƒgƒ‹‰æ‘œ
	_start_button->draw( ); 
	_manual_button->draw( );
	_drawer->flip( );
}