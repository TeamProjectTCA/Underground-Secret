#include "Map.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "CharaA.h"
#include "const.h"
#include "Debug.h"
#include "Shutter.h"
#include "Scroll.h"
#include <errno.h>
#include <assert.h>

const int COLLIDER_ASCIICODE_MIN = '0';
const int ELEVATOR_ASCIICODE_MIN = 'A';
const int ELEVATOR_ASCIICODE_MAX = 'z';
const int ENDPOINT_ASCIICODE = '6';
const int SHUTTER_ASCIICODE = '2';
const unsigned int COLLIDER_COLOR = GREEN;
const unsigned int ENDPOINT_COLOR = MAGENTA;
const unsigned int ELEVATOR_COLOR = YELLOW;
const unsigned int SHUTTER_COLOR  = WATER;
const int SHUTTER_MOVECOUNT_MAX = FPS;

std::string path = "Resources/map/stage";

Map::Map( int stage, ScrollConstPtr scroll ) :
_stage( stage ),
_scroll( scroll ) {
	_debug = Debug::getTask( );
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_shutter = ShutterPtr( new Shutter( ) );

	std::string stage_path = "stage" + std::to_string( _stage );
	_map_handle = _drawer->getImage( stage_path );

	_col = 0;
	_row = 0;
	_data = "";
	_debug_mode = false;
	_fixedpoint_alpha_start = Vector( );
	_fixedpoint_alpha_play  = Vector( );
	_fixedpoint_alpha_end   = Vector( );
	_fixedpoint_beta_start = Vector( );
	_fixedpoint_beta_play  = Vector( );
	_fixedpoint_beta_end   = Vector( );

	// .colファイルから読み込み
	loadMap( );
	setFixedpoint( );
	setShutter( );

	_shutter->setCol( _col );
}

Map::~Map( ) {
}

void Map::update( ) {
	_shutter->setScroll( _scroll->getScroll( ) );
	_shutter->update( );

	// debug
	_debug_mode = _debug->isDebug( );
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

Vector Map::getElevatorPos( int ascii ) const {
	char find = ascii;
	int idx = ( int )_data.find_first_of( find );
	if ( idx == std::string::npos ) {
		return Vector( -1, -1, -1 );
	}
	return Vector(
		( idx % _col ) * BLOCK_SIZE,
		( idx / _col ) * BLOCK_SIZE 
	);
}

Vector Map::getScrollData( ) const {
	return _scroll->getScroll( );
}

int Map::getCol( ) const {
	return _col;
}

int Map::getRow( ) const {
	return _row;
}

int Map::getMapData( int idx ) const {
	if ( idx >= ( int )_data.length( ) ) {
		return -1;
	}

	return _data[ idx ];
}

bool Map::isHitShutter( int detection_idx ) const {
	return _shutter->isHitShutter( detection_idx );
}

void Map::draw( ) const {
	// マップを描画
	Vector scroll = _scroll->getScroll( );
	_drawer->drawGraph( ( int )scroll.x * BLOCK_SIZE, ( int )scroll.y * BLOCK_SIZE, _map_handle, true );

	//シャッターを描画
	_shutter->draw( );

	// debug
	if ( _debug_mode ) {
		drawCollider( );
		drawTable( );
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
		// \nを消す
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
			// 定点alphaの入力であれば
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

void Map::setShutter( ) {
	int length = ( int )_data.length( );
	int shutter_num = 0;
	for ( int i = 0; i < length; i++ ) {
		if ( _data[ i ] != SHUTTER_ASCIICODE ) {
			continue;
		}

		// シャッターを登録
		std::vector< int > shutter;
		shutter.push_back( i );
		
		// 直下を判定し、当たり判定が見つかるまでをシャッターとする
		inputShutter( shutter, i );

		// シャッターをaddする
		_shutter->addShutter( shutter );
	}
}

void Map::inputShutter( std::vector< int > &shutter, int idx ) {
	int point = idx + _col;

	if ( _data[ point ] - COLLIDER_ASCIICODE_MIN != IDENTIFICATION_NONE ) {
		return;
	}

	// 1つしたが何もなければシャッターの判定を付ける
	shutter.push_back( point );

	// 再起してもう1つ下を見る
	inputShutter( shutter, point );
}
void Map::drawCollider( ) const {
	Vector scroll = _scroll->getScroll( );
	int range_width_min = ( int )scroll.x * -1;
	int range_width_max = range_width_min + WIDTH / BLOCK_SIZE;
	int range_height_min = ( int )scroll.y * -1;
	int range_height_max = range_height_min + HEIGHT / BLOCK_SIZE;

	for ( int i = 0; i < _row; i++ ) {

		// 画面内のものだけ描画
		if ( i < range_height_min ) {
			continue;
		}
		if ( i > range_height_max ) {
			break;
		}

		for ( int j = 0; j < _col; j++ ) {
			// 画面内のものだけ描画
			if ( j < range_width_min ) {
				continue;
			}
			if ( j > range_width_max ) {
				break;
			}

			int idx = i * _col + j;

			// 何もなければ次を見る
			int ascii = _data[ idx ];
			if ( ascii - COLLIDER_ASCIICODE_MIN == IDENTIFICATION_NONE ) {
				continue;
			}

			unsigned int color = COLLIDER_COLOR;
			int x = idx % _col;
			int y = idx / _col;

			// 色
			if ( ELEVATOR_ASCIICODE_MIN <= ascii && ascii <= ELEVATOR_ASCIICODE_MAX ) {
				color = ELEVATOR_COLOR;
			}
			if ( ascii == ENDPOINT_ASCIICODE ) {
				color = ENDPOINT_COLOR;
			}
			if ( ascii == SHUTTER_ASCIICODE ) {
				color = SHUTTER_COLOR;
			}

			_drawer->drawBox( ( float )(     x + scroll.x ) * BLOCK_SIZE, ( float )(     y + scroll.y ) * BLOCK_SIZE,
				              ( float )( x + 1 + scroll.x ) * BLOCK_SIZE, ( float )( y + 1 + scroll.y ) * BLOCK_SIZE,
				              color, true );
		}
	}
}

void Map::drawTable( ) const {
	for ( int i = 1; i < HEIGHT / BLOCK_SIZE; i++ ) {
		_drawer->drawLine( ( float )0, ( float )i * BLOCK_SIZE, ( float )WIDTH, ( float )i * BLOCK_SIZE, GRAY );
	}
	for ( int i = 1; i < WIDTH / BLOCK_SIZE; i++ ) {
		_drawer->drawLine( ( float )i * BLOCK_SIZE, ( float )0, ( float )i * BLOCK_SIZE, ( float )HEIGHT, GRAY );
	}
}