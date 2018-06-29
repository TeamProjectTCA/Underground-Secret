#include "SceneStageSelect.h"
#include "Keyboard.h"
#include "Drawer.h"
#include "const.h"
#include "Button.h"
#include "Vector.h"
#include "Mouse.h"

const float BUTTON_Y = HEIGHT / 4 * 3;
const float BUTTON_PITCH = WIDTH / 4;
const int BUTTON_WIDTH = 256;
const int BUTTON_HEIGHT = 256;
const float STAGE_STRING_Y = HEIGHT / 4 * 1.5;
const float STAGE_STRING_HEIGHT = 128;

const char BUTTON_STAGE1_IMAGE[] = "stage1";
const char BUTTON_STAGE2_IMAGE[] = "stage2";
const char BUTTON_STAGE3_IMAGE[] = "stage3";
const char *BUTTON_IMAGE[ STAGE_MAX ] = { BUTTON_STAGE1_IMAGE, BUTTON_STAGE2_IMAGE, BUTTON_STAGE3_IMAGE };

const char BACK_IMAGE[ ] = "underground_back";
const char BUTTON_FRAME_IMAGE[ ] = "stagebutton_frame";
const char STAGE1_STRING_IMAGE[ ] = "stage1_string";
const char STAGE2_STRING_IMAGE[ ] = "stage2_string";
const char STAGE3_STRING_IMAGE[ ] = "stage3_string";
const char *STAGE_STRING_IMAGE[ STAGE_MAX ] = { STAGE1_STRING_IMAGE, STAGE2_STRING_IMAGE, STAGE3_STRING_IMAGE };

const char LOG_IMAGE[ ] = "stageselect";

SceneStageSelect::SceneStageSelect( ) {
	_stage = 1;
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_mouse = Mouse::getTask( );

	_back_image = _drawer->getImage( BACK_IMAGE );
	_buttonframe_image = _drawer->getImage( BUTTON_FRAME_IMAGE );
	createButton( );
}

SceneStageSelect::~SceneStageSelect( ) {
}

void SceneStageSelect::update( ) {
	Vector mouse_pos = _mouse->getPoint( );
	for ( int i = 0; i < _button.size( ); i++ ) {
		if ( _mouse->getClickingLeft( ) ) {
			_button[ i ]->click( mouse_pos );
		} else {
			if ( _button[ i ]->isPush( ) ) {
				_stage = i + 1;
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

	// log
	int log = _drawer->getImage( LOG_IMAGE );
	float log_height = ( float )_drawer->getImageHeight( LOG_IMAGE );
	float lx = WIDTH / 4.0f;
	float ly = HEIGHT / 5.0f - log_height / 2.0f;
	float rx = WIDTH / 4.0f * 3.0f;
	float ry = HEIGHT / 5.0f + log_height / 2.0f;
	_drawer->drawExtendGraph( lx, ly, rx, ry, log, true );

	const int FRAME_GAP = 20;
	for ( int i = 0; i < _button.size( ); i++ ) {
		_button[ i ]->draw( );

		// frame
		Vector left = _button[ i ]->getColliderLeft( );
		Vector right = _button[ i ]->getColliderRight( );
		_drawer->drawExtendGraph( 
			( float )left.x  - FRAME_GAP, ( float )left.y  - FRAME_GAP, 
			( float )right.x + FRAME_GAP, ( float )right.y + FRAME_GAP, 
			_buttonframe_image, true );

		// string
		int handle = _drawer->getImage( STAGE_STRING_IMAGE[ i ] );
		_drawer->drawExtendGraph(
			( float )left.x , STAGE_STRING_Y,
			( float )right.x, STAGE_STRING_Y + STAGE_STRING_HEIGHT,
			handle, true );
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