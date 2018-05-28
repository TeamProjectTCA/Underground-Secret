#include "Debug.h"
#include "Manager.h"
#include "Keyboard.h"
#include "const.h"
#include "Drawer.h"

const int FONT_SIZE = 20;
const int MAX_ROW = HEIGHT / FONT_SIZE;
const int LOG_X = 20;

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
	_drawer = Drawer::getTask( );
	_log.push_back( "test" );
}

void Debug::update( ) {
	if ( _keyboard->getKeyDown( "SPACE" ) ) {
		_debug = !_debug;
	}
}

void Debug::draw( ) const {
	if ( !_debug ) {
		return;
	}
	drawLog( );
}

void Debug::drawLog( ) const {
	std::list< std::string >::const_iterator ite;
	ite = _log.begin( );
	int y = FONT_SIZE;
	for ( ite; ite != _log.end( ); ite++ ) {
		_drawer->drawString( LOG_X, y, ( *ite ).c_str( ), RED );
		y += FONT_SIZE;
	}
}

void Debug::addLog( std::string log ) {
	int size = ( int )_log.size( );
	if ( size == MAX_ROW ) {
		_log.push_front( log );
		_log.pop_back( );
		return;
	}

	_log.push_back( log );
}

void Debug::clearLog( ) {
	std::list< std::string >( ).swap( _log );
}

bool Debug::isDebug( ) const {
	return _debug;
}