#include "SceneStageSelect.h"
#include "Keyboard.h"
#include "Drawer.h"
#include "const.h"
#include "Button.h"
#include "Vector.h"
#include "Mouse.h"

const Vector BUTTON_POSITION = Vector( WIDTH / 2, HEIGHT / 2 );
const char BUTTON_NORMAL_IMAGE[ ] = "GameStart";
const char BUTTON_PUSH_IMAGE  [ ] = "GameStartClick";

SceneStageSelect::SceneStageSelect( ) {
	_stage = 1;
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );

	int button_width  = _drawer->getImageWidth( BUTTON_NORMAL_IMAGE );
	int button_height = _drawer->getImageHeight( BUTTON_NORMAL_IMAGE );
	_button = ButtonPtr( new Button( 
		( float )BUTTON_POSITION.x - button_width  / 2.0f, 
		( float )BUTTON_POSITION.y - button_height / 2.0f,
		( float )BUTTON_POSITION.x + button_width  / 2.0f,
		( float )BUTTON_POSITION.y + button_height / 2.0f ) );

	_button->setImage( BUTTON_NORMAL_IMAGE );
	_button->setPushImage( BUTTON_PUSH_IMAGE );
}

SceneStageSelect::~SceneStageSelect( ) {
}

void SceneStageSelect::update( ) {
	if ( _keyboard->getKeyDown( "z" ) ) {
		setNextScene( SCENE_GAME );
	}

	MousePtr mouse = Mouse::getTask( );
	Vector mouse_pos = mouse->getPoint( );
	if ( mouse->getClickingLeft( ) ) {
		_button->click( mouse_pos );
	} else {
		if ( _button->isPush( ) ) {
			setNextScene( SCENE_GAME );
		}
		_button->resetState( );
	}

	draw( );
}

void SceneStageSelect::draw( ) const {
	_button->draw( );
	_drawer->flip( );
}

int SceneStageSelect::getStage( ) const {
	return _stage;
}