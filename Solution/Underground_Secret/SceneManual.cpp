#include "SceneManual.h"
#include "Keyboard.h"
#include "Drawer.h"
#include "const.h"
#include "Button.h"
#include "Vector.h"
#include "Mouse.h"

SceneManual::SceneManual( ) {
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_mouse = Mouse::getTask( );
}

SceneManual::~SceneManual( ) {
}

void SceneManual::update( ) {
	if ( _keyboard->getKeyDown( "z" ) ) {
		setNextScene( SCENE_TITLE );
	}
	draw( );
}

void SceneManual::draw( ) const {
	// ”wŒi
	_drawer->drawExtendGraph( 0, 0, WIDTH_F, HEIGHT_F, _drawer->getImage( "underground_back" ), true ); //”wŒi
	_drawer->drawExtendGraph( 240, 60, 1040, 660, _drawer->getImage( "board" ), true ); 

	_drawer->flip( );
}