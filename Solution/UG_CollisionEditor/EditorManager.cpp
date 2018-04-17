#include "EditorManager.h"
#include "Manager.h"
#include "Keyboard.h"
#include "ImageLoad.h"
#include <array>

EditorManagerPtr EditorManager::getTask( ) {
	return std::dynamic_pointer_cast< EditorManager >( Manager::getInstance( )->getTask( getTag( ) ) );
}

std::string EditorManager::getTag( ) {
	return "EDITOR_MANAGER";
}

EditorManager::EditorManager( ) {
	_command = COMMAND_NONE;
}

EditorManager::~EditorManager( ) {
}

void EditorManager::initialize( ) {
	_keyboard = Keyboard::getTask( );
	_image_load = ImageLoadPtr( new ImageLoad( ) );
}

void EditorManager::finalize( ) {
}

void EditorManager::update( ) {
	switch ( _command ) {
	case COMMAND_NONE:
		break;

	case COMMAND_IMAGE_LOAD:
		_image_load->update( );
		break;

	default:
		break;
	}

	//コマンドをチェック
	std::array< int, COMMAND_MAX > command = { 1, 2, 5, 6 };
	for ( int i = 0; i < COMMAND_MAX; i++ ) {
		if ( _keyboard->isKeyDownFunction( command[ i ] ) ) {
			_command = ( COMMAND )i;
			break;
		}
	}
}