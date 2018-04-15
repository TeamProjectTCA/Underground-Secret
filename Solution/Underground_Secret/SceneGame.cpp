#include "SceneGame.h"
#include "Keyboard.h"
#include "Drawer.h"

SceneGame::SceneGame( ) {
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
}

SceneGame::~SceneGame( ) {
}

void SceneGame::update( ) {
	_drawer->drawString( 10, 10, "SceneGame", _drawer->getColor( 255, 0, 0 ) );

	if ( _keyboard->getKeyDown( "x" ) ) {
		setNextScene( TITLE );
	}
}
