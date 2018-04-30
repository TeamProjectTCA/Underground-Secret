#include "SceneGame.h"
#include "Keyboard.h"
#include "Drawer.h"
#include "Map.h"
#include "CharacterManager.h"

SceneGame::SceneGame( int stage ) :
_stage( stage ) {
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_map = MapPtr( new Map( _stage ) );
	_char_manager = CharacterManagerPtr( new CharacterManager( ) );
}

SceneGame::~SceneGame( ) {
}

void SceneGame::update( ) {
	_char_manager->update( );
	_map->update( );

	_drawer->drawString( 10, 10, "SceneGame", 0xff0000 );

	if ( _keyboard->getKeyDown( "x" ) ) {
		setNextScene( TITLE );
	}

	_drawer->flip( );
}
