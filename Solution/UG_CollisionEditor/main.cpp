#include "Manager.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "EditorManager.h"

void main( ) {
	ManagerPtr manager = Manager::getInstance( );
	DrawerPtr drawer( new Drawer( "Import" ) );
	KeyboardPtr keyboard( new Keyboard( ) );
	MousePtr mouse( new Mouse( ) );
	EditorManagerPtr editor( new EditorManager( ) );

	manager->add( Drawer::getTag( ), drawer );
	manager->add( Keyboard::getTag( ), keyboard );
	manager->add( Mouse::getTag( ), mouse );
	manager->add( EditorManager::getTag( ), editor );
}