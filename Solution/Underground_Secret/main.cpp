#include "Manager.h"
#include "DxSetting.h"
#include "Drawer.h"
#include "SceneManager.h"
#include "Keyboard.h"
#include "Mouse.h"

void main( ) {
	ManagerPtr manager = Manager::getInstance( );
	DxSettingPtr setting = DxSetting::getInstance( );
	setting->changeWindowMode( true );
	setting->setGraphMode( 1280, 720, 32 );
	setting->setWindowSize( 1280, 720 );
	setting->setDrawScreenBack( );

	DrawerPtr drawer( new Drawer( "Resources/image" ) );
	SceneManagerPtr scene_manager( new SceneManager( ) );
	KeyboardPtr keyboard( new Keyboard( ) );
	MousePtr mouse( new Mouse( ) );

	//add
	manager->add( Drawer::getTag( ), drawer );
	manager->add( SceneManager::getTag( ), scene_manager );
	manager->add( Keyboard::getTag( ), keyboard );
	manager->add( Mouse::getTag( ), mouse );
}