#include "Manager.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "EditorManager.h"

void main( ) {
	ManagerPtr manager = Manager::getInstance( );
	KeyboardPtr keyboard( new Keyboard( ) );
	MousePtr mouse( new Mouse( ) );
	EditorManagerPtr editor( new EditorManager( ) );

	manager->add( Keyboard::getTag( ), keyboard );
	manager->add( Mouse::getTag( ), mouse );
	manager->add( EditorManager::getTag( ), editor );
}