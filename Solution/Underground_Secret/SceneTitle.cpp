#include "SceneTitle.h"
#include "Drawer.h"
#include "Sound.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Button.h"
#include "const.h"

const Vector BUTTON_POSITION = Vector( WIDTH / 2, HEIGHT / 5 * 4 );
const char BUTTON_NORMAL_IMAGE[ ] = "GameStart";
const char BUTTON_PUSH_IMAGE[ ] = "GameStartClick";
const char BACKGROUND_IMAGE[ ] = "underground_back";
const char TITLE_IMAGE[ ] = "title";

SceneTitle::SceneTitle( ) {
	_drawer = Drawer::getTask( );
	_sound = Sound::getTask( );
	_keyboard = Keyboard::getTask( );
	_mouse = Mouse::getTask( );
	_background_handle = _drawer->getImage( BACKGROUND_IMAGE );
	_title_handle = _drawer->getImage( TITLE_IMAGE );

	_cur_hand = LoadCursor( NULL, IDC_HAND );
	_startbutton = BoxObject( );

	//ボタン
	int button_width = _drawer->getImageWidth( BUTTON_NORMAL_IMAGE );
	int button_height = _drawer->getImageHeight( BUTTON_NORMAL_IMAGE );
	_button = ButtonPtr( new Button( 
		( float )BUTTON_POSITION.x - button_width / 2.0f,
		( float )BUTTON_POSITION.y - button_height / 2.0f,
	   	( float )BUTTON_POSITION.x + button_width / 2.0f,
		( float )BUTTON_POSITION.y + button_height / 2.0f ) );
	_button->setPos( ( float )BUTTON_POSITION.x, ( float )BUTTON_POSITION.y );
	_button->setImage( BUTTON_NORMAL_IMAGE );
	_button->setPushImage( BUTTON_PUSH_IMAGE );

	//BGM
	_sound_handle[ TITLE_BGM ] = _sound->load( "TitleBGM/titlebgm.ogg" );
	_sound->play( _sound_handle[ TITLE_BGM ], true, true );

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
		_button->click( mouse_pos );
	} else {
		if ( _button->isPush( ) ) {
			_sound->stop( _sound_handle[ TITLE_BGM ] );
			_sound->play( _sound_handle[ BUTTON_SE ] );
			setNextScene( SCENE_STAGESELECT );
		}
		_button->resetState( );
	}

	draw( );

}

void SceneTitle::draw( ) const {
	_drawer->drawExtendGraph( 0, 0, WIDTH_F, HEIGHT_F, _background_handle, true ); //背景
	_drawer->drawRotaGraph( WIDTH / 2, HEIGHT / 7 * 2, 1, 0, _title_handle, true ); //タイトル画像
	_button->draw( ); //ボタン
	_drawer->flip( );
}