#include "Map.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "CharaA.h"
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
	_charaA = CharaAPtr( new CharaA( ) );
	_handle = _drawer->getImage( ( "stage" + std::to_string( _stage ) ).c_str( ) );
}

Map::~Map( ) {
}

void Map::update( ) {
	_drawer->drawGraph( 0, 0, _handle, true );

	if ( _debug ) {
		drawCollider( );
		drawTable( );
	}

	if ( _keyboard->getKeyDown( "SPACE" ) ) {
		_debug = !_debug;
	}
	ColliderSet( );
	_charaA->update( );
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

void Map::drawCollider( ) const {
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
				              0x00ff00, true );
		}
	}
}

void Map::drawTable( ) const {
	for ( int i = 1; i < HEIGHT / BLOCK_SIZE; i++ ) {
		_drawer->drawLine( ( float )0, ( float )i * BLOCK_SIZE, ( float )WIDTH, ( float )i * BLOCK_SIZE, 0x3b3b3b );
	}
	for ( int i = 1; i < WIDTH / BLOCK_SIZE; i++ ) {
		_drawer->drawLine( ( float )i * BLOCK_SIZE, ( float )0, ( float )i * BLOCK_SIZE, ( float )HEIGHT, 0x3b3b3b );
	}
}

void Map::ColliderSet( ) {
	int _check_x_down       = ( _charaA->getPosX( ) + _charaA->getWidth( )  / 2 ) / BLOCK_SIZE;
	int _check_y_down       = ( _charaA->getPosY( ) + _charaA->getHeight( ) + 1 ) / BLOCK_SIZE;
	int _check_x_left       = ( _charaA->getPosX( ) - 1 ) / BLOCK_SIZE;
	int _check_x_right      = ( _charaA->getPosX( ) + _charaA->getWidth( )  + 1 ) / BLOCK_SIZE;
	int _check_y_left_right = ( _charaA->getPosY( ) + _charaA->getHeight( ) - 1 ) / BLOCK_SIZE;
	
	
	if ( _data[ _check_y_down * _col + _check_x_down ] != '0' ) {
		_charaA->setFall( false );
		if ( _charaA->getDir( ) == DIR_LEFT ) {
			if ( _data[ _check_y_left_right * _col + _check_x_left ] != '0' ) {
				if ( _data[ ( _check_y_left_right - 1 ) * _col + _check_x_left ] != '0' ) {
					_charaA->setMove( false );
				} else {
					_charaA->setMove( true );
					_charaA->setMoveUp( true );
				}
			} else {
				_charaA->setMove( true );
			}
		}
		if ( _charaA->getDir( ) == DIR_RIGHT ) {
			if ( _data[ _check_y_left_right * _col + _check_x_right ] != '0' ) {
				if ( _data[ ( _check_y_left_right - 1 ) * _col + _check_x_right ] != '0' ) {
					_charaA->setMove( false );
				} else {
					_charaA->setMove( true );
					_charaA->setMoveUp( true );
				}
			} else {
				_charaA->setMove( true );
			}
		}
	} else {
		_charaA->setFall( true );
	}
}