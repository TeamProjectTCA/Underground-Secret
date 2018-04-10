#include "SceneGame.h"
#include "Keyboard.h"

SceneGame::SceneGame( ) {
	_keyboard = Keyboard::getTask( );
}

SceneGame::~SceneGame( ) {
}

void SceneGame::update( ) {

	if ( _keyboard->getKeyDown( "z" ) ) {
		setNextScene( TITLE );
	}
}
