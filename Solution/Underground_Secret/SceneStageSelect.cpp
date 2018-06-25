#include "SceneStageSelect.h"
#include "Keyboard.h"
#include "Drawer.h"
#include "const.h"
#include "Button.h"
#include "Vector.h"
#include "Mouse.h"

const char BUTTON_STAGE1_IMAGE[ ] = "stage1";
const char *BUTTON_IMAGE[ STAGE_MAX ] = { BUTTON_STAGE1_IMAGE, BUTTON_STAGE1_IMAGE, BUTTON_STAGE1_IMAGE };
const int BUTTON_WIDTH = 256;
const int BUTTON_HEIGHT = 256;
const float BUTTON_PITCH = WIDTH / 4;
const float BUTTON_Y = HEIGHT / 3 * 2;

const char BACK_IMAGE[ ] = "underground_back";

SceneStageSelect::SceneStageSelect( ) {
	_stage = 1;
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_mouse = Mouse::getTask( );

	_back_image = _drawer->getImage( BACK_IMAGE );
	createButton( );
}

SceneStageSelect::~SceneStageSelect( ) {
}

void SceneStageSelect::update( ) {
	if ( _keyboard->getKeyDown( "z" ) ) {
		setNextScene( SCENE_GAME );
	}

	Vector mouse_pos = _mouse->getPoint( );
	for ( int i = 0; i < _button.size( ); i++ ) {
		if ( _mouse->getClickingLeft( ) ) {
			_button[ i ]->click( mouse_pos );
		} else {
			if ( _button[ i ]->isPush( ) ) {
				setNextScene( SCENE_GAME );
			}
			_button[ i ]->resetState( );
		}
	}

	draw( );
}

void SceneStageSelect::draw( ) const {
	// ”wŒi
	_drawer->drawGraph( 0, 0, _back_image, true );

	for ( int i = 0; i < _button.size( ); i++ ) {
		_button[ i ]->draw( );
	}
	_drawer->flip( );
}

int SceneStageSelect::getStage( ) const {
	return _stage;
}

void SceneStageSelect::createButton( ) {
	for ( int i = 0; i < STAGE_MAX; i++ ) {
		ButtonPtr button( new Button( ) );

		button->setPos(
			( float )BUTTON_PITCH * ( i + 1 ) - BUTTON_WIDTH / 2, ( float )BUTTON_Y - BUTTON_HEIGHT / 2,
			( float )BUTTON_PITCH * ( i + 1 ) + BUTTON_WIDTH / 2, ( float )BUTTON_Y + BUTTON_HEIGHT / 2 );

		button->setCollider(
			( float )BUTTON_PITCH * ( i + 1 ) - BUTTON_WIDTH / 2, ( float )BUTTON_Y - BUTTON_HEIGHT / 2, 
			( float )BUTTON_PITCH * ( i + 1 ) + BUTTON_WIDTH / 2, ( float )BUTTON_Y + BUTTON_HEIGHT / 2 );

		button->setImage    ( BUTTON_IMAGE[ i ] );
		button->setPushImage( BUTTON_IMAGE[ i ] );

		_button.push_back( button );
	}
}