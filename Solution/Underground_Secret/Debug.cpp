#include "Debug.h"
#include "Manager.h"
#include "Keyboard.h"

DebugPtr Debug::getTask( ) {
	return std::dynamic_pointer_cast< Debug >( Manager::getInstance( )->getTask( getTag( ) ) );
}

std::string Debug::getTag( ) {
	return "DEBUG";
}

Debug::Debug( ) {
	_debug = false;
}

Debug::~Debug( ) {
}

void Debug::initialize( ) {
	_keyboard = Keyboard::getTask( );
}

void Debug::update( ) {
	if ( _keyboard->getKeyDown( "SPACE" ) ) {
		_debug = !_debug;
	}
}

bool Debug::isDebug( ) const {
	return _debug;
}