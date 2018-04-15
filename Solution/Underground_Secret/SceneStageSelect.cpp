#include "SceneStageSelect.h"
#include "Keyboard.h"
#include "Drawer.h"

SceneStageSelect::SceneStageSelect( ) {
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
}

SceneStageSelect::~SceneStageSelect( ) {
}

void SceneStageSelect::update( ) {
	_drawer->drawString( 10, 10, "SceneStageSelect", 0xff0000 );

	if ( _keyboard->getKeyDown( "z" ) ) {
		setNextScene( GAME );
	}
}