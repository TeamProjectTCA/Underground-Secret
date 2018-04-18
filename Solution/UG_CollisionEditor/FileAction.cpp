#include "FileAction.h"
#include "ImageLoad.h"
#include "Table.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Drawer.h"

FileAction::FileAction( std::string action_name ) :
_action_name( action_name ) {
	_keyboard = Keyboard::getTask( );
	_mouse = Mouse::getTask( );
	_drawer = Drawer::getTask( );
	_fin = false;
}

FileAction::~FileAction( ) {
}

void FileAction::update( ) {
	writeName( );
	if ( _keyboard->isEnterKey( ) ) {
		actionEnter( );
		std::string( ).swap( _input );
	}
}

void FileAction::writeName( ) {
	_drawer->drawString( 20, 20, _action_name.c_str( ), 0xff0000 );
	_drawer->drawString( 20, 80, "ƒtƒ@ƒCƒ‹–¼‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢", 0xffffff );

	std::string key = _keyboard->getString( );
	if ( key != "" ) {
		if ( _keyboard->getKeyDown( key ) ) {
			_input += key;
		}
	}

	if ( _keyboard->getBackSpace( ) == 1 || _keyboard->getBackSpace( ) > 30 ) {
		int end = ( int )_input.length( );
		if ( end != 0 ) {
			_input.pop_back( );
		}
	}

	_drawer->drawFormatString( 20, 100, 0xff5555, _input.c_str( ) );
	_drawer->flip( );
}

bool FileAction::isFin( ) const {
	return _fin;
}