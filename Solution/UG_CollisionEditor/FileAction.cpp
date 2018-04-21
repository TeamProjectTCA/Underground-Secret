#include "FileAction.h"
#include "FileImageLoad.h"
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
	_enter = false;
	_gauge_up = false;
}

FileAction::~FileAction( ) {
}

void FileAction::update( ) {
	if ( _action_name != "FileExport" ) {
		writeName( );
	}

	if ( _gauge_up ) {
		upGauge( );
	}

	if ( _keyboard->getKeyDown( "ENTER" ) ) {
		_enter = true;
	}

	if ( _enter ) {
		actionEnter( );
		std::string( ).swap( _input );
		_enter = false;
	}
	_drawer->flip( );
}

void FileAction::setGauge( ) {
	_gauge_up = true;
}

void FileAction::writeName( ) {
	_drawer->drawString( 20, 20, _action_name.c_str( ), 0xff0000 );
	_drawer->drawString( 20, 80, "ファイル名を入力してください", 0xffffff );

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

	// F8で戻る
	_drawer->drawString( 20, 300, "F8キーで戻ります", 0xffff00 );
}

void FileAction::upGauge( ) {
}

bool FileAction::isFin( ) const {
	return _fin;
}