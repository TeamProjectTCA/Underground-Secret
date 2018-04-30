#include "Map.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "const.h"
#include <errno.h>
#include <assert.h>

std::string path = "Resources/map/stage";

Map::Map( int stage ) :
_stage( stage ) {
	_col = 0;
	_row = 0;
	_data = "";
	_debug = false;
	loadMap( );

	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_handle = _drawer->getImage( ( "stage" + std::to_string( _stage ) ).c_str( ) );
}

Map::~Map( ) {
}

void Map::update( ) {
	_drawer->drawGraph( 0, 0, _handle, true );

	if ( _debug ) {
		drawCollider( );
	}

	if ( _keyboard->getKeyDown( "SPACE" ) ) {
		_debug = !_debug;
	}
}

void Map::loadMap( ) {
	FILE *fp;
	errno_t file_open = fopen_s( &fp, ( path + std::to_string( _stage ) + "/collision_data.col" ).c_str( ), "r" );
	assert( file_open == 0 );

	int col = 0;
	int row = 0;
	while ( true ) {
		char buf[ 2048 ] = { };
		if ( fgets( buf, 2048, fp ) == NULL ) {
			break;
		}

		std::string str = buf;
		// \nÇè¡Ç∑
		str = str.substr( 0, str.length( ) - 1 );

		_data += str;
		col = ( int )str.length( );
		row++;
	}

	fclose( fp );
	_col = col;
	_row = row;
}

void Map::drawCollider( ) {
	for ( int i = 0; i < _row; i++ ) {
		for ( int j = 0; j < _col; j++ ) {
			int idx = i * _col + j;
			if ( _data[ idx ] == '0' ) {
				continue;
			}

			int x = idx % _col;
			int y = idx / _col;

			_drawer->drawBox( ( float )(     x ) * BLOCK_SIZE, ( float )(     y ) * BLOCK_SIZE, 
				              ( float )( x + 1 ) * BLOCK_SIZE, ( float )( y + 1 ) * BLOCK_SIZE,
				              0xff0000, true );
		}
	}
}