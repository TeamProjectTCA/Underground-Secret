#include "Character.h"
#include "Drawer.h"
#include "Map.h"
#include "const.h"
#include <assert.h>
#include <errno.h>

const int DEFAULT_ANIM_TIME = 30;
const int DEFAULT_MAX_COUNT = 0xffffff;
const int COLLIDER_ASCIICODE_MIN = '0';
const int ELEVATOR_ASCIICODE_MIN = 'A';
const int COLLIDER_ASCIICODE_MAX = '9';
const int ELEVATOR_ASCIICODE_MAX = 'z';
const int ENDPOINT_ASCIICODE = '6';
const int CHANGE_ASCIICODE = 'a' - 'A';

Character::Character( MapPtr map, std::vector< std::string > info ) :
_map( map ),
_info( info ) {
	_drawer = Drawer::getTask( );
	
	_anim_type = WALK;
	_anim_change_time = DEFAULT_ANIM_TIME;

	_cnt = 0;
	_sx = 0;
	_max_cnt = DEFAULT_MAX_COUNT;
	_pos = Vector( );
	_scroll = Vector( );
	_debug = false;
}

Character::~Character( ) {
}

void Character::addAnim( ANIM_TYPE type, std::string filename, int frame_num ) {
	int handle = _drawer->getImage( filename );
	errno_t addAnim = handle;
	assert( addAnim != -1 );

	_anim[ type ].handle = handle;
	_anim[ type ].frame = frame_num;
	_anim[ type ].height = _drawer->getImageHeight( filename );
	_anim[ type ].width = _drawer->getImageWidth( filename ) / frame_num;
}

void Character::setAnim( ANIM_TYPE type ) {
	if ( _anim.find( type ) == _anim.end( ) ) {
		return;
	}

	_anim_type = type;
	_max_cnt = _anim[ _anim_type ].frame * _anim_change_time;
	_cnt = 0;
}

void Character::setAnimTime( int change_time ) {
	_anim_change_time = change_time;
	_max_cnt = _anim[ _anim_type ].frame * _anim_change_time;
}

void Character::setFixedpoint( PHASE phase ) {
	_pos = _map->getFixedpointAlpha( phase );
}

void Character::changeDebugMode( ) {
	_debug = !_debug;
}

void Character::move( Vector move ) {
	_pos += move;
}

bool Character::isLooking( Vector pos ) const {
	int width  = _anim.find( _anim_type )->second.width / 2;
	int height = _anim.find( _anim_type )->second.height / 2;
	Vector central    = pos + _scroll + Vector( width, height );
	Vector left_up    = central + Vector( -width, -height ); 
	Vector right_up   = central + Vector(  width, -height );
	Vector left_down  = central + Vector( -width,  height );
	Vector right_down = central + Vector(  width,  height );
	if ( ( 0 <=    left_up.x &&    left_up.x <= WIDTH && 0 <=    left_up.y &&    left_up.y <= HEIGHT) ||
		 ( 0 <=  left_down.x &&  left_down.x <= WIDTH && 0 <=  left_down.y &&  left_down.y <= HEIGHT) ||
		 ( 0 <=   right_up.x &&   right_up.x <= WIDTH && 0 <=   right_up.y &&   right_up.y <= HEIGHT) ||
		 ( 0 <= right_down.x && right_down.x <= WIDTH && 0 <= right_down.y && right_down.y <= HEIGHT) ) {
		return true;
	}
	return false;
}

bool Character::isEndpoint( Vector pos ) const {
	int width = _anim.find(_anim_type)->second.width / 2;
	int height = _anim.find(_anim_type)->second.height / 2;
	Vector central = pos + Vector( width, height );
	int idx = ( int )( central.x / BLOCK_SIZE ) + ( int )( central.y / BLOCK_SIZE ) * _map->getCol();
	int data = _map->getMapData( idx );

	if ( data == ENDPOINT_ASCIICODE ) {
		return true;
	}
	return false;
}

Vector Character::getPos( ) const {
	return _pos;
}

int Character::getMapDataCollider( Vector pos ) const {
	int data = -1;

	Vector position = pos;
	position.x += ( int )_anim.find( _anim_type )->second.width / 2;
	position.y += ( int )_anim.find( _anim_type )->second.height - 1; //ぴったりになってしまうのを防ぐ

	int idx = ( int )( position.x / BLOCK_SIZE ) + ( int )( position.y / BLOCK_SIZE ) * _map->getCol( );

	data = _map->getMapData( idx );
	
	// エレベーター若しくは終点であったら進行可能
	if ( ( ELEVATOR_ASCIICODE_MIN <= data && data <= ELEVATOR_ASCIICODE_MAX ) || data == ENDPOINT_ASCIICODE ) {
		return IDENTIFICATION_NONE;
	}

	data -= COLLIDER_ASCIICODE_MIN;

	// 当たり判定を見ている場所を表示(debug)
	 if ( _debug ) {
		 // 左右
		 const unsigned int CHECK_COLOR_X = BLUE;
		 const unsigned int CHECK_COLOR_Y = YELLOW;
		 unsigned int color = CHECK_COLOR_X;
		 if ( pos.y > _pos.y || pos.y < _pos.y ) {
			 color = CHECK_COLOR_Y;
		 }

		_drawer->drawBox(
			( float )( idx % _map->getCol( ) + _scroll.x ) * BLOCK_SIZE, 
			( float )( idx / _map->getCol( ) + _scroll.y ) * BLOCK_SIZE,
			( float )( idx % _map->getCol( ) + _scroll.x + 1 ) * BLOCK_SIZE, 
			( float )( idx / _map->getCol( ) + _scroll.y + 1 ) * BLOCK_SIZE,
			color, true );
	 }

	return data;
}

int Character::getMapDataElevator( Vector pos ) const {
	int data = -1;

	Vector position = pos;
	position.x += ( int )_anim.find( _anim_type )->second.width / 2;
	position.y += ( int )_anim.find( _anim_type )->second.height - 1; //ぴったりになってしまうのを防ぐ

	int idx = ( int )( position.x / BLOCK_SIZE ) + ( int )( position.y / BLOCK_SIZE ) * _map->getCol( );

	data = _map->getMapData( idx );

	// エレベーター以外であったら
	if ( COLLIDER_ASCIICODE_MIN <= data && data <= COLLIDER_ASCIICODE_MAX ) {
		return -1;
	}

	return data;
}


void Character::draw( ) {
	if ( _anim.find( _anim_type ) == _anim.end( ) ) {
		return;
	}
	
	_scroll = _map->getScrollData( );
	_cnt = ( _cnt + 1 ) % _max_cnt;

	if ( _cnt % _anim_change_time == 0 ) {
		// 画像を切り替える( 左端をずらす )
		_sx = ( ( _cnt / _anim_change_time ) % _anim[ _anim_type ].frame ) * _anim[ _anim_type ].width;
	}

	_drawer->drawRectGraph( 
		( float )( _pos.x + _scroll.x * BLOCK_SIZE ), ( float )( _pos.y + _scroll.y * BLOCK_SIZE ),
		_sx, 0,
		_anim[ _anim_type ].width, _anim[ _anim_type ].height, 
		_anim[ _anim_type ].handle,
		true, false );
}

void Character::setFallPos( Vector now_position ) {
	Vector position = now_position;

	position.y += ( int )_anim.find( _anim_type )->second.height;
	int idx = ( int )( position.y / BLOCK_SIZE );
	position.y = ( idx * BLOCK_SIZE ) - _anim[ _anim_type ].height;

	_pos.y = position.y;
}

void Character::setPos( Vector pos ) {
	_pos = pos;
}

void Character::setElevatorPos( int ascii ) {
	if ( 'a' <= ascii && ascii <= 'z' ) {
		ascii += -CHANGE_ASCIICODE;
	} else {
		ascii +=  CHANGE_ASCIICODE;
	}
	Vector pos = _map->getElevatorPos( ascii );
	if ( pos.x < 0 ) {
		return;
	}

	_pos = pos;
}

void Character::checkCaughtCollider( ) {
	Vector position = _pos;
	position.x += ( int )_anim.find( _anim_type )->second.width / 2;
	position.y += ( int )_anim.find( _anim_type )->second.height - 1;

	int idx = ( int )( position.x / BLOCK_SIZE ) + ( int )( position.y / BLOCK_SIZE ) * _map->getCol( );

	int data = -1;
	data = _map->getMapData( idx );
	data -= COLLIDER_ASCIICODE_MIN;

	if ( data != IDENTIFICATION_COLLIDER ) {
		return;
	}

	_pos = Vector( _pos.x, ( idx / _map->getCol( ) ) * BLOCK_SIZE  - _anim[ _anim_type ].height );


	// 再起
	checkCaughtCollider( );
}