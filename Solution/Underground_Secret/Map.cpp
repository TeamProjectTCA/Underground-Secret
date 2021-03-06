#include "Map.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "const.h"
#include "Debug.h"
#include "Shutter.h"
#include "Scroll.h"
#include "Elevator.h"
#include "SpecialElevator.h"
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
const char ELEVATOR_ALPHA_IMAGE[ ] = "elevator_start";
const char ELEVATOR_BETA_IMAGE [ ] = "elevator_goal";

Map::Map( int stage, ScrollConstPtr scroll ) :
_stage( stage ),
_scroll( scroll ),
_endpoint( -1 ),
_fixedpoint_alpha_start( Vector( ) ),
_fixedpoint_alpha_play ( Vector( ) ),
_fixedpoint_alpha_end  ( Vector( ) ),
_fixedpoint_beta_start ( Vector( ) ),
_fixedpoint_beta_play  ( Vector( ) ),
_fixedpoint_beta_end   ( Vector( ) ) {
	_debug = Debug::getTask( );
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_shutter = ShutterPtr( new Shutter( stage ) );

	std::string stage_path = "stage" + std::to_string( _stage );
	_map_handle = _drawer->getImage( stage_path );

	_col = 0;
	_row = 0;
	_data = "";
	_debug_mode = false;


	// .colファイルから読み込み
	loadMap( );
	setFixedpoint( );
	setShutter( );
	setElevator( );
	setEndpoint( );

	_shutter->setCol( _col );

	_special_elevator_alpha = SpecialElevatorPtr( new SpecialElevator( _scroll ) );
	_special_elevator_beta  = SpecialElevatorPtr( new SpecialElevator( _scroll ) );
	_special_elevator_alpha->setImage( ELEVATOR_ALPHA_IMAGE );
	_special_elevator_beta ->setImage( ELEVATOR_BETA_IMAGE  );
	_special_elevator_alpha->setElevator( getFixedpointBeta( PHASE_START ), getFixedpointAlpha( PHASE_PLAY ) );
	_special_elevator_beta ->setElevator( Vector( _endpoint % _col * BLOCK_SIZE, _endpoint / _col * BLOCK_SIZE + BLOCK_SIZE ), getFixedpointBeta( PHASE_END   ) );

}

Map::~Map( ) {
}

void Map::update( ) {
	_shutter->setScroll( _scroll->getScroll( ) );
	_shutter->update( );

	// elevator
	std::unordered_map< char, ElevatorPtr >::iterator ite;
	ite = _elevator.begin( );
	for ( ite; ite != _elevator.end( ); ite++ ) {
		ite->second->setScroll( _scroll->getScroll( ) );
		ite->second->update( );
	}

	// special elevator
	_special_elevator_alpha->update( );
	_special_elevator_beta->update( );

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

	point.y += BLOCK_SIZE;

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

	point.y += BLOCK_SIZE;

	return point;
}

Vector Map::getElevatorPos( char id ) const {
	if ( _elevator.find( id ) == _elevator.end( ) ) {
		return Vector( -1, -1, -1 );
	}

	ElevatorPtr elevator = _elevator.find( id )->second;
	int idx = elevator->getDestinationPos( );

	if ( idx < 0 ) {
		return Vector( -1, -1, -1 );
	}

	return Vector(
		( idx % _col ) * BLOCK_SIZE,
		( idx / _col ) * BLOCK_SIZE 
	);
}

int Map::getElevatorIdx( char id ) const {
	if ( _elevator.find( id ) == _elevator.end( ) ) {
		return -1;
	}
	ElevatorPtr elevator = _elevator.find( id )->second;
	return elevator->getActivePos( );
}

Vector Map::getScrollData( ) const {
	return _scroll->getScroll( );
}

Vector Map::getMapPos( ) const {
	return Vector( ( int )( getScrollData( ).x ), ( int )( getScrollData( ).y ) );
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

SpecialElevatorPtr Map::getSpecialElevatorAlphaPtr( ) {
	return _special_elevator_alpha;
}

SpecialElevatorPtr Map::getSpecialElevatorBetaPtr( ) {
	return _special_elevator_beta;
}

char Map::getElevatorId( int idx ) const {
	char id = getMapData( idx );

	if ( _elevator.find( id ) == _elevator.end( ) ) {
		return 0x00;
	}

	ElevatorPtr elevator = _elevator.find( id )->second;
	int up     = elevator->getElevatorPos( ELEVATOR_POS_UP );
	int center = elevator->getElevatorPos( ELEVATOR_POS_CENTER );
	int down   = elevator->getElevatorPos( ELEVATOR_POS_DOWN );

	if ( up     == idx ||
		 center == idx ||
		 down   == idx ) {
		return id;
	}

	return 0x00;
}

ELEVATOR_STATE Map::getElevatorState( char id, int idx ) const {
	if ( _elevator.find( id ) == _elevator.end( ) ) {
		return ELEVATOR_STATE_NONE;
	}

	ElevatorPtr elevator = _elevator.find( id )->second;
	//if ( elevator->getElevatorPos( elevator->getActiveElevator( ) ) != idx ) {
	//	return ELEVATOR_STATE_NONE;
	//}

	return elevator->getElevatorState( );
}

ELEVATOR_POS Map::getElevatorPos( char id, int idx ) const {
	if ( getElevatorState( id, idx ) == ELEVATOR_STATE_NONE ) {
		return ELEVATOR_POS( );
	}
	ElevatorPtr elevator = _elevator.find( id )->second;
	return elevator->getElevatorPos( idx );
}

ELEVATOR_POS Map::getActiveElevator( char id, int idx ) const {
	if ( getElevatorState( id, idx ) == ELEVATOR_STATE_NONE ) {
		return ELEVATOR_POS( );
	}
	ElevatorPtr elevator = _elevator.find( id )->second;
	return elevator->getActiveElevator( );
}

ELEVATOR_POS Map::getDestination( char id, int idx ) const {
	if ( getElevatorState( id, idx ) == ELEVATOR_STATE_NONE ) {
		return ELEVATOR_POS( );
	}
	ElevatorPtr elevator = _elevator.find( id )->second;
	return elevator->getDestination( );
}

void Map::draw( ) const {
	// map
	Vector scroll = _scroll->getScroll( );
	_drawer->drawGraph( ( int )( scroll.x ), ( int )( scroll.y ), _map_handle, true );

	// shutter
	_shutter->draw( );

	// elevator
	std::unordered_map< char, ElevatorPtr >::const_iterator ite;
	ite = _elevator.begin( );
	for ( ite; ite != _elevator.end( ); ite++ ) {
		ite->second->draw( );
	}

	// special elevator
	_special_elevator_alpha->draw( );
	_special_elevator_beta->draw( );

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

void Map::setElevator( ) {
	int length = ( int )_data.length( );
	for ( int i = 0; i < length; i++ ) {
		char id = _data[ i ];
		if ( id < ELEVATOR_ASCIICODE_MIN || ELEVATOR_ASCIICODE_MAX < id ) {
			continue;
		}
		if ( _elevator.find( id ) == _elevator.end( ) ) {
			_elevator[ id ] = ElevatorPtr( new Elevator( _col, id ) );
		}
		_elevator[ id ]->add( i );
	}
}

void Map::setEndpoint( ) {
	for ( int i = 0; i < _data.length( ); i++ ) {
		if ( _data[ i ] == ENDPOINT_ASCIICODE ) {
			_endpoint = i;
			return;
		}
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
	int range_width_min = ( int )scroll.x / 16 * -1;
	int range_width_max = range_width_min + WIDTH / BLOCK_SIZE;
	int range_height_min = ( int )scroll.y / 16 * -1;
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

			_drawer->drawBox( ( float )( (     x ) * BLOCK_SIZE + scroll.x ),
				              ( float )( (     y ) * BLOCK_SIZE + scroll.y ),
				              ( float )( ( x + 1 ) * BLOCK_SIZE + scroll.x ),
				              ( float )( ( y + 1 ) * BLOCK_SIZE + scroll.y ),
				              color, true );
		}
	}
}

void Map::drawTable( ) const {
	Vector scroll = _scroll->getScroll( );
	for ( int i = 1; i < HEIGHT / BLOCK_SIZE; i++ ) {
		float sx = 0.0f;
		float sy = ( float )( i * BLOCK_SIZE );
		float ex = WIDTH_F;
		float ey = sy;
		_drawer->drawLine( sx, sy, ex, ey, GRAY );
	}
	for ( int i = 1; i < WIDTH / BLOCK_SIZE; i++ ) {
		float sx = ( float )( i * BLOCK_SIZE );
		float sy = 0.0f;
		float ex = sx;
		float ey = HEIGHT_F;
		_drawer->drawLine( sx, sy, ex, ey, GRAY );
	}
}