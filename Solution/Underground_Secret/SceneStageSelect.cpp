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
	_drawer->drawString( 10, 10, "SceneStageSelect", _drawer->getColor( 255, 0, 0 ) );

	if ( _keyboard->getKeyDown( "z" ) ) {
		setNextScene( GAME );
	}
}