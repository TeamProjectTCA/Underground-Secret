#include "SceneStageSelect.h"
#include "Keyboard.h"
#include "Drawer.h"
#include "const.h"
#include "Button.h"
#include "Vector.h"
#include "Mouse.h"
#include "Sound.h"

const float LOG_PITCH = 230;

const float BUTTON_STAGE_Y = HEIGHT / 4 * 2.5;
const float BUTTON_STAGE_PITCH = WIDTH / 4;
const int BUTTON_STAGE_WIDTH = 256;
const int BUTTON_STAGE_HEIGHT = 256;
const float STAGE_STRING_PITCH = 128;
const float STAGE_STRING_Y = BUTTON_STAGE_Y - STAGE_STRING_PITCH - BUTTON_STAGE_HEIGHT / 2.0f;

const char BUTTON_RETURN_IMAGE     [ ] = "Return";
const char BUTTON_RETURNCLICK_IMAGE[ ] = "ReturnClick";
const float BUTTON_RETURN_WIDTH  = WIDTH_F / 4.0f * 1.5f;
const float BUTTON_RETURN_HEIGHT =  64;
const float BUTTON_RETURN_X = WIDTH_F / 2.0f;
const float BUTTON_RETURN_Y = HEIGHT_F - BUTTON_RETURN_HEIGHT;

const char BUTTON_STAGE1_IMAGE     [ ] = "stage1";
const char BUTTON_STAGE2_IMAGE     [ ] = "stage2";
const char BUTTON_STAGE3_IMAGE     [ ] = "stage3";
const char *BUTTON_IMAGE[ STAGE_MAX ] = { BUTTON_STAGE1_IMAGE, BUTTON_STAGE2_IMAGE, BUTTON_STAGE3_IMAGE };

const char BACK_IMAGE[ ] = "underground_back";
const char BUTTON_FRAME_IMAGE [ ] = "stagebutton_frame";
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
	// stage button
	bool mouse_click = ( _mouse->getClickingLeft( ) > 0 ? true : false );
	for ( int i = 0; i < _stage_button.size( ); i++ ) {
		if ( mouse_click ) {
			_stage_button[ i ]->click( _mouse->getPoint( ) );
		} else {
			if ( _stage_button[ i ]->isPush( ) ) {
				_stage = i + 1;
				SoundPtr sound = Sound::getTask( );
				sound->play( sound->load( "SoundEffect/button.ogg" ), false, true );
				setNextScene( SCENE_GAME );
			}
			_stage_button[ i ]->resetState( );
		}
	}

	// return button
	if ( mouse_click ) {
		_return_button->click( _mouse->getPoint( ) );
	} else {
		if ( _return_button->isPush( ) ) {
			setNextScene( SCENE_TITLE );
			SoundPtr sound = Sound::getTask( );
			sound->play( sound->load( "SoundEffect/button.ogg" ), false, true );
		}
		_return_button->resetState( );
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
	float ry = ly + LOG_PITCH;
	_drawer->drawExtendGraph( lx, ly, rx, ry, log, true );

	// stage button
	const int FRAME_GAP = 20;
	for ( int i = 0; i < _stage_button.size( ); i++ ) {
		_stage_button[ i ]->draw( );

		// frame
		Vector left  = _stage_button[ i ]->getColliderLeft( );
		Vector right = _stage_button[ i ]->getColliderRight( );
		_drawer->drawExtendGraph( 
			( float )left.x  - FRAME_GAP, ( float )left.y  - FRAME_GAP, 
			( float )right.x + FRAME_GAP, ( float )right.y + FRAME_GAP, 
			_buttonframe_image, true );

		// string
		int handle = _drawer->getImage( STAGE_STRING_IMAGE[ i ] );
		_drawer->drawExtendGraph(
			( float )left.x , STAGE_STRING_Y,
			( float )right.x, STAGE_STRING_Y + STAGE_STRING_PITCH,
			handle, true );
	}

	// return button
	_return_button->draw( false );

	_drawer->flip( );
}

int SceneStageSelect::getStage( ) const {
	return _stage;
}

void SceneStageSelect::createButton( ) {
	// stage button
	for ( int i = 0; i < STAGE_MAX; i++ ) {
		ButtonPtr button( new Button( ) );

		button->setPos(
			( float )BUTTON_STAGE_PITCH * ( i + 1 ) - BUTTON_STAGE_WIDTH / 2, ( float )BUTTON_STAGE_Y - BUTTON_STAGE_HEIGHT / 2,
			( float )BUTTON_STAGE_PITCH * ( i + 1 ) + BUTTON_STAGE_WIDTH / 2, ( float )BUTTON_STAGE_Y + BUTTON_STAGE_HEIGHT / 2 );

		button->setCollider(
			( float )BUTTON_STAGE_PITCH * ( i + 1 ) - BUTTON_STAGE_WIDTH / 2, ( float )BUTTON_STAGE_Y - BUTTON_STAGE_HEIGHT / 2, 
			( float )BUTTON_STAGE_PITCH * ( i + 1 ) + BUTTON_STAGE_WIDTH / 2, ( float )BUTTON_STAGE_Y + BUTTON_STAGE_HEIGHT / 2 );

		button->setImage    ( BUTTON_IMAGE[ i ] );
		button->setPushImage( BUTTON_IMAGE[ i ] );

		_stage_button.push_back( button );
	}

	// return button
	_return_button = ButtonPtr( new Button( ) );
	_return_button->setImage( BUTTON_RETURN_IMAGE );
	_return_button->setPushImage( BUTTON_RETURNCLICK_IMAGE );
	_return_button->setPos(
		BUTTON_RETURN_X - BUTTON_RETURN_WIDTH / 2, BUTTON_RETURN_Y - BUTTON_RETURN_HEIGHT / 2,
		BUTTON_RETURN_X + BUTTON_RETURN_WIDTH / 2, BUTTON_RETURN_Y + BUTTON_RETURN_HEIGHT / 2 );
	_return_button->setCollider(
		BUTTON_RETURN_X - BUTTON_RETURN_WIDTH / 2, BUTTON_RETURN_Y - BUTTON_RETURN_HEIGHT / 2,
		BUTTON_RETURN_X + BUTTON_RETURN_WIDTH / 2, BUTTON_RETURN_Y + BUTTON_RETURN_HEIGHT / 2 );
}