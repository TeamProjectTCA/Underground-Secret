#include "Manager.h"
#include "Drawer.h"
#include "Sound.h"
#include "SceneManager.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Debug.h"
#include "Random.h"
#include "const.h"

void main( ) {
	ManagerPtr manager = Manager::getInstance( );
	//manager->changeWindowMode( false );
	manager->setScreenSize( WIDTH, HEIGHT );
	manager->setWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );

	DrawerPtr drawer( new Drawer( "Resources/image" ) );
	SoundPtr soundplayer( new Sound( "Resources/sound" ) );
	SceneManagerPtr scene_manager( new SceneManager( ) );
	KeyboardPtr keyboard( new Keyboard( ) );
	MousePtr mouse( new Mouse( ) );
	DebugPtr debug( new Debug( ) );
	RandomPtr random( new Random( ) );

	//add
	manager->add( Drawer::getTag( ), drawer );
	manager->add( Sound::getTag( ), soundplayer );
	manager->add( SceneManager::getTag( ), scene_manager );
	manager->add( Keyboard::getTag( ), keyboard );
	manager->add( Mouse::getTag( ), mouse );
	manager->add( Debug::getTag( ), debug );
	manager->add( Random::getTag( ), random );
}