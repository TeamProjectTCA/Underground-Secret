#include "Character.h"
#include "Drawer.h"
#include "Map.h"
#include "const.h"
#include "Debug.h"
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
const int SCREEN_WIDTH_BLOCK_NUM = WIDTH / BLOCK_SIZE;
const int SCREEN_HEIGHT_BLOCK_NUM = HEIGHT / BLOCK_SIZE;

Character::Character( MapPtr map, std::vector< std::string > info ) :
_map( map ),
_info( info ),
_anim_type( ANIM_WALK ),
_anim_change_time( DEFAULT_ANIM_TIME ),
_anim_cnt( 0 ),
_sx( 0 ),
_max_cnt( DEFAULT_MAX_COUNT ),
_pos( Vector( ) ),
_scroll( Vector( ) ) {
	_drawer = Drawer::getTask( );
	_debug = Debug::getTask( );
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
	_anim_cnt = 0;
}

void Character::setAnimTime( int change_time ) {
	_anim_change_time = change_time;
	_max_cnt = _anim[ _anim_type ].frame * _anim_change_time;
}

void Character::setFixedpoint( PHASE phase ) {
	Vector pos = _map->getFixedpointAlpha( phase );
	_pos = pos;
}

void Character::setScroll( ) {
	_scroll = _map->getScrollData( );
}

void Character::move( Vector move ) {
	_pos += move;
}

bool Character::isLooking( Vector pos ) const {
	// キャラの中心のブロックの x, y
	int x = ( int )( pos.x / BLOCK_SIZE );
	int y = ( int )( ( pos.y - _anim.find( _anim_type )->second.height / 2 ) / BLOCK_SIZE );

	// スクリーンの左上座標
	int screen_x = ( int )( _scroll.x * -1 );
	int screen_y = ( int )( _scroll.y * -1 );

	if ( screen_x < x && x < screen_x + SCREEN_WIDTH_BLOCK_NUM &&
		 screen_y < y && y < screen_y + SCREEN_HEIGHT_BLOCK_NUM ) {
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
	position.y -= 1; //ぴったりになってしまうのを防ぐ

	int idx = ( int )( position.x / BLOCK_SIZE ) + ( int )( position.y / BLOCK_SIZE ) * _map->getCol( );

	data = _map->getMapData( idx );
	
	// エレベーターであったら進行可能
	if ( ELEVATOR_ASCIICODE_MIN <= data && data <= ELEVATOR_ASCIICODE_MAX ) {
		return IDENTIFICATION_NONE;
	}

	// endpoint
	if ( data == ENDPOINT_ASCIICODE ) {
		return IDENTIFICATION_ENDPOINT;
	}

	// シャッターの判定
	if ( _map->isHitShutter( idx ) ) {
		return IDENTIFICATION_SHUTTER;
	}

	data -= COLLIDER_ASCIICODE_MIN;

	return data;
}

int Character::getMapDataElevator( Vector pos ) const {
	int data = -1;

	Vector position = pos;
	position.y -= 1; //ぴったりになってしまうのを防ぐ

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

	// カウントを進める
	_anim_cnt = ( _anim_cnt + 1 ) % _max_cnt;

	if ( _anim_cnt % _anim_change_time == 0 ) {
		// 画像を切り替える( 左端をずらす )
		_sx = ( ( _anim_cnt / _anim_change_time ) % _anim[ _anim_type ].frame ) * _anim[ _anim_type ].width;
	}


	Vector pos = Vector( _pos.x - _anim[ _anim_type ].width / 2, _pos.y - _anim[ _anim_type ].height );
	_drawer->drawRectGraph( 
		( float )( pos.x + _scroll.x * BLOCK_SIZE ), ( float )( pos.y + _scroll.y * BLOCK_SIZE ),
		_sx, 0,
		_anim[ _anim_type ].width, _anim[ _anim_type ].height, 
		_anim[ _anim_type ].handle,
		true, false );
}

void Character::setFallPos( Vector now_position ) {
	Vector position = now_position;

	int idx = ( int )( position.y / BLOCK_SIZE );

	// ぴったりにそろえる
	position.y = ( idx * BLOCK_SIZE );

	_pos.y = position.y;
}

std::vector< std::string > &Character::getInfo( ) {
	return _info;
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
	position.y -= 1;

	int idx = ( int )( position.x / BLOCK_SIZE ) + ( int )( position.y / BLOCK_SIZE ) * _map->getCol( );

	int data = -1;
	data = _map->getMapData( idx );
	data -= COLLIDER_ASCIICODE_MIN;

	if ( data != IDENTIFICATION_COLLIDER ) {
		return;
	}

	_pos = Vector( _pos.x, ( idx / _map->getCol( ) ) * BLOCK_SIZE );


	// 再起
	checkCaughtCollider( );
}