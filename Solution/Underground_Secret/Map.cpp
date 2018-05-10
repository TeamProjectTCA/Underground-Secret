#include "Map.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "CharaA.h"
#include "const.h"
#include <errno.h>
#include <assert.h>

const int SCROLL_SIZE = 2;

std::string path = "Resources/map/stage";

Map::Map( int stage ) :
_stage( stage ) {
	_col = 0;
	_row = 0;
	_data = "";
	_debug = false;
	_fixedpoint_alpha_start = Vector( );
	_fixedpoint_alpha_play  = Vector( );
	_fixedpoint_alpha_end   = Vector( );
	_fixedpoint_beta_start = Vector( );
	_fixedpoint_beta_play  = Vector( );
	_fixedpoint_beta_end   = Vector( );
	_scroll = Vector( 0, 0 );

	loadMap( );
	setFixedpoint( );

	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_handle = _drawer->getImage( ( "stage" + std::to_string( _stage ) ).c_str( ) );
}

Map::~Map( ) {
}

void Map::update( ) {
	_drawer->drawGraph( ( int )_scroll.x * BLOCK_SIZE, ( int )_scroll.y * BLOCK_SIZE, _handle, true );
	scroll( );
	if ( _debug ) {
		drawCollider( );
		drawTable( );
	}

	if ( _keyboard->getKeyDown( "SPACE" ) ) {
		_debug = !_debug;
	}
}

Vector Map::getFixedpointAlpha( PHASE phase ) const {
	Vector point;

	switch ( phase ) {
	case PHASE_START: point = _fixedpoint_alpha_start; break;
	case PHASE_PLAY : point = _fixedpoint_alpha_play ; break;
	case PHASE_END  : point = _fixedpoint_alpha_end  ; break;
	default: break;
	}

	return point;
}

Vector Map::getFixedpointBeta( PHASE phase ) const {
	Vector point;

	switch ( phase ) {
	case PHASE_START: point = _fixedpoint_beta_start; break;
	case PHASE_PLAY : point = _fixedpoint_beta_play ; break;
	case PHASE_END  : point = _fixedpoint_beta_end  ; break;
	default: break;
	}

	return point;
}


Vector Map::getScrollData( ) const {
	return Vector( _scroll.x, _scroll.y );
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

void Map::setFixedpoint( ) {
	int length = ( int )_data.length( );
	Vector *vec = NULL;
	bool alphapos_input_start = false;
	bool alphapos_input_play  = false;
	bool alphapos_input_end   = false;

	for ( int i = 0; i < length; i++ ) {
		bool detection = false;
		int value = _data[ i ] - '0';
		if ( value == IDENTIFICATION_FIXEDPOINT_START ) {
			// íËì_alphaÇÃì¸óÕÇ≈Ç†ÇÍÇŒ
			if ( !alphapos_input_start ) {
				vec = &_fixedpoint_alpha_start;
				alphapos_input_start = true;
			} else {
				vec = &_fixedpoint_beta_start;
			}
			detection = true;
		}
		if ( value == IDENTIFICATION_FIXEDPOINT_PLAY ) {
			if ( !alphapos_input_play ) {
				vec = &_fixedpoint_alpha_play;
				alphapos_input_play = true;
			} else {
				vec = &_fixedpoint_beta_play;
			}
			detection = true;
		}
		if ( value == IDENTIFICATION_FIXEDPOINT_END ) {
			if ( !alphapos_input_end ) {
				vec = &_fixedpoint_alpha_end;
				alphapos_input_end = true;
			} else {
				vec = &_fixedpoint_beta_end;
			}
			detection = true;
		}
		if ( !detection ) {
			continue;
		}

		vec->x = ( i % _col ) * BLOCK_SIZE;
		vec->y = ( i / _col ) * BLOCK_SIZE;
	}
}

void Map::scroll( ) {
	//ç∂
	if ( _keyboard->getKeyDown( "d" ) || _keyboard->getState( "d" ) > 30 ) {
		_scroll.x -= SCROLL_SIZE;
	}
	//âE
	if ( _keyboard->getKeyDown( "a" ) || _keyboard->getState( "a" ) > 30 ) {
		_scroll.x += SCROLL_SIZE;
	}
	//è„
	if ( _keyboard->getKeyDown( "s" ) || _keyboard->getState( "s" ) > 30 ) {
		_scroll.y -= SCROLL_SIZE;
	}
	//â∫
	if ( _keyboard->getKeyDown( "w" ) || _keyboard->getState( "w" ) > 30 ) {
		_scroll.y += SCROLL_SIZE;
	}


	if ( _scroll.x > 0 ) {
		_scroll.x = 0;
	}
	if ( _scroll.x < WIDTH / BLOCK_SIZE - ( _col ) ) {
		_scroll.x = WIDTH / BLOCK_SIZE - ( _col );
	}

	if ( _scroll.y > 0 ) {
		_scroll.y = 0;
	}
	if ( _scroll.y < HEIGHT / BLOCK_SIZE - ( _row ) ) {
		_scroll.y = HEIGHT / BLOCK_SIZE - ( _row );
	}
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

			_drawer->drawBox( ( float )(     x + _scroll.x ) * BLOCK_SIZE, ( float )(     y + _scroll.y ) * BLOCK_SIZE,
				              ( float )( x + 1 + _scroll.x ) * BLOCK_SIZE, ( float )( y + 1 + _scroll.y ) * BLOCK_SIZE,
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

//void Map::ColliderSet( ) {
//	int _check_x_down       = ( _charaA->getPosX( ) + _charaA->getWidth( )  / 2 ) / BLOCK_SIZE;
//	int _check_y_down       = ( _charaA->getPosY( ) + _charaA->getHeight( ) + 1 ) / BLOCK_SIZE;
//	int _check_x_left       = ( _charaA->getPosX( ) - 1 ) / BLOCK_SIZE;
//	int _check_x_right      = ( _charaA->getPosX( ) + _charaA->getWidth( )  + 1 ) / BLOCK_SIZE;
//	int _check_y_left_right = ( _charaA->getPosY( ) + _charaA->getHeight( ) - 1 ) / BLOCK_SIZE;
//	
//	if ( _data[ _check_y_down * _col + _check_x_down ] != '0' ) {
//		_charaA->setFall( false );
//		if ( _charaA->getDir( ) == DIR_LEFT ) {
//			if ( _data[ _check_y_left_right * _col + _check_x_left ] != '0' ) {
//				if ( _data[ ( _check_y_left_right - 1 ) * _col + _check_x_left ] != '0' ) {
//					_charaA->setMove( false );
//				} else {
//					_charaA->setMove( true );
//					_charaA->setMoveUp( true );
//				}
//			} else {
//				_charaA->setMove( true );
//			}
//		}
//		if ( _charaA->getDir( ) == DIR_RIGHT ) {
//			if ( _data[ _check_y_left_right * _col + _check_x_right ] != '0' ) {
//				if ( _data[ ( _check_y_left_right - 1 ) * _col + _check_x_right ] != '0' ) {
//					_charaA->setMove( false );
//				} else {
//					_charaA->setMove( true );
//					_charaA->setMoveUp( true );
//				}
//			} else {
//				_charaA->setMove( true );
//			}
//		}
//	} else {
//		_charaA->setFall( true );
//	}
//}