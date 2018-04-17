#include "FileAction.h"
#include "ImageLoad.h"
#include "MassCollider.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Drawer.h"

FileAction::FileAction( ) {
	_keyboard = Keyboard::getTask( );
	_mouse = Mouse::getTask( );
	_drawer = Drawer::getTask( );
}

FileAction::~FileAction( ) {
}

void FileAction::update( ) {
	writeName( );
	if ( _keyboard->isEnterKey( ) ) {
		actionEnter( );
	}
	if ( _enter ) {
		drawLoadedImage( );
		drawMass( );
		drawMassSelect( );
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

void FileAction::drawLoadedImage( ) const {
	_drawer->drawGraph( 0, 0, _image.handle, false );
}

void FileAction::drawMass( ) const {
	for ( int i = 1; i < _image.height / 16; i++ ) {
		_drawer->drawLine( ( float )0, ( float )i * 16, ( float )_image.width, ( float )i * 16, 0xff0000 );
	}
	for ( int i = 1; i < _image.width / 16; i++ ) {
		_drawer->drawLine( ( float )i * 16, ( float )0, ( float )i * 16, ( float )_image.height, 0xff0000 );
	}
}

void FileAction::drawMassSelect( ) const {
	int x = _mouse->getPointX( );
	int y = _mouse->getPointY( );
	for ( int i = 0; i < _image.width; i++ ) {
		for ( int j = 0; j < _image.height; j++ ) {
			if ( i * 16 < x && x < ( i + 1 ) * 16 &&
				 j * 16 < y && y < ( j + 1 ) * 16 &&
				 x <= _image.width &&
				 y <= _image.height ) {
				_drawer->drawBox( ( float )i * 16, ( float )j * 16, ( float )( i + 1 ) * 16, ( float )( j + 1 ) * 16, 0xff0000, true );
			}

		}
	}
}