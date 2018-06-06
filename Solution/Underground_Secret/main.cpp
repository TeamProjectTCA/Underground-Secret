#include "Manager.h"
#include "DxSetting.h"
#include "Drawer.h"
#include "Soundplayer.h"
#include "SceneManager.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Debug.h"
#include "Random.h"

void main( ) {
	ManagerPtr manager = Manager::getInstance( );
	DxSettingPtr setting = DxSetting::getInstance( );
	setting->changeWindowMode( true );
	setting->setGraphMode( 1280, 720, 60 );
	setting->setWindowSize( 1280, 720 );
	setting->setDrawScreenBack( );

	DrawerPtr drawer( new Drawer( "Resources/image" ) );
	SoundplayerPtr soundplayer( new Soundplayer( "Resources/sound" ) );
	SceneManagerPtr scene_manager( new SceneManager( ) );
	KeyboardPtr keyboard( new Keyboard( ) );
	MousePtr mouse( new Mouse( ) );
	DebugPtr debug( new Debug( ) );
	RandomPtr random( new Random( ) );

	//add
	manager->add( Drawer::getTag( ), drawer );
	manager->add( Soundplayer::getTag( ), soundplayer );
	manager->add( SceneManager::getTag( ), scene_manager );
	manager->add( Keyboard::getTag( ), keyboard );
	manager->add( Mouse::getTag( ), mouse );
	manager->add( Debug::getTag( ), debug );
	manager->add( Random::getTag( ), random );
}