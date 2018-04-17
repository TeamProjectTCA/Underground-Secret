#include "FileAction.h"
#include "Keyboard.h"
#include "Drawer.h"

FileAction::FileAction( ) {
	_keyboard = Keyboard::getTask( );
	_drawer = Drawer::getTask( );
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
