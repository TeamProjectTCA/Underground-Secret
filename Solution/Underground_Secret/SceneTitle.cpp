#include "SceneTitle.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "const.h"

SceneTitle::SceneTitle( ) {
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_background_handle = _drawer->getImage( "back_001" );
}

SceneTitle::~SceneTitle( ) {
}

void SceneTitle::update( ) {
	_drawer->drawRotaGraph( WIDTH / 2, HEIGHT / 2, 1, 0, _background_handle, true );

	if ( _keyboard->getKeyDown( "z" ) ) {
		setNextScene( GAME );
	}
}
