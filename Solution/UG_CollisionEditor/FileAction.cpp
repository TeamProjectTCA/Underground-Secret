#include "FileAction.h"
#include "ImageLoad.h"
#include "Table.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Drawer.h"

FileAction::FileAction( ) {
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
	}
}

void FileAction::writeName( ) {
	_drawer->flip( );
	_drawer->drawString( 20, 20, "ƒtƒ@ƒCƒ‹–¼‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢", 0xffffff );

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

	_drawer->drawFormatString( 20, 50, 0xff0000, _input.c_str( ) );
}

bool FileAction::isFin( ) const {
	return _fin;
}