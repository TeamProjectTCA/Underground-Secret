#include "SceneStageSelect.h"
#include "Keyboard.h"
#include "Drawer.h"
#include "const.h"

SceneStageSelect::SceneStageSelect( ) {
	_stage = 1;
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
}

SceneStageSelect::~SceneStageSelect( ) {
}

void SceneStageSelect::update( ) {
	if ( _keyboard->getKeyDown( "z" ) ) {
		setNextScene( SCENE_GAME );
	}

	_drawer->flip( );
}

int SceneStageSelect::getStage( ) const {
	return _stage;
}