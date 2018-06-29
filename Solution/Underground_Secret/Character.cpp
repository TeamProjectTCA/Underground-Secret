#include "Character.h"
#include "Drawer.h"
#include "Map.h"
#include "const.h"
#include "Debug.h"
#include <assert.h>
#include <errno.h>

const int DEFAULT_ANIM_TIME = FPS / 2;
const int DEFAULT_MAX_COUNT = 0xffffff;

const int COLLIDER_ASCIICODE_MIN = '0';
const int ELEVATOR_ASCIICODE_MIN = 'A';
const int COLLIDER_ASCIICODE_MAX = '9';
const int ELEVATOR_ASCIICODE_MAX = 'z';
const int ENDPOINT_ASCIICODE = '6';
const int CHANGE_ASCIICODE = 'a' - 'A';

const int FONT_SIZE = 20;

const char INFO_IMAGE[ ] = "person_hint";

Character::Character( MapPtr map, std::vector< std::string > info ) :
_map( map ),
_info( info ),
_anim_type( ANIM_WALK ),
_anim_change_time( DEFAULT_ANIM_TIME ),
_anim_cnt( 0 ),
_sx( 0 ),
_max_cnt( DEFAULT_MAX_COUNT ),
_pos( Vector( ) ),
_scroll( Vector( ) ),
_show_info_num( 0 ),
_spy( false ),
_draw_flag( true ),
_move_dir( MOVE_DIR_RIGHT ) {
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

void Character::setSpy( bool spy ) {
	_spy = spy;
}

void Character::addShowInfoNum( ) {
	int num = _show_info_num + 1;

	if ( num < ( int )_info.size( ) + 1 ) {
		_show_info_num = num;
	}
}

void Character::move( Vector move ) {
	Vector past = _pos;
	_pos += move;
	if ( ( _pos - past ).x < 0 ) {
		_move_dir = MOVE_DIR_LEFT;
	} else {
		_move_dir = MOVE_DIR_RIGHT;
	}
}

bool Character::isLooking( Vector pos ) const {
	// キャラの中心のブロックの x, y
	int x = ( int )   pos.x;
	int y = ( int ) ( pos.y - _anim.find( _anim_type )->second.height / 2 );

	// スクリーンの左上座標
	int screen_x = ( int )( _scroll.x * -1 );
	int screen_y = ( int )( _scroll.y * -1 );

	if ( screen_x < x && x < screen_x + WIDTH &&
		 screen_y < y && y < screen_y + HEIGHT ) {
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

bool Character::isSpy( ) const {
	return _spy;
}

Character::ANIM_TYPE Character::getAnimType( ) const {
	return _anim_type;
}

Vector Character::getPos( ) const {
	return _pos;
}

int Character::getMapData( Vector pos ) const {
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

char Character::getElevatorId( ) const {
	Vector position = _pos;
	position.y -= 1; //ぴったりになってしまうのを防ぐ
	int idx = ( int )( position.x / BLOCK_SIZE ) + ( int )( position.y / BLOCK_SIZE ) * _map->getCol( );
	return _map->getElevatorId( idx );
}

bool Character::isActiveElevatorPos( ) const {
	Vector position = _pos;
	position.y -= 1; //ぴったりになってしまうのを防ぐ
	int idx = ( int )( position.x / BLOCK_SIZE ) + ( int )( position.y / BLOCK_SIZE ) * _map->getCol( );
	int elevator_idx = _map->getElevatorIdx( getElevatorId( ) );
	if ( idx == elevator_idx ) {
		return true;
	}
	return false;
}

ELEVATOR_STATE Character::getElevatorState( ) const {
	Vector position = _pos;
	position.y -= 1; //ぴったりになってしまうのを防ぐ
	int idx = ( int )( position.x / BLOCK_SIZE ) + ( int )( position.y / BLOCK_SIZE ) * _map->getCol( );
	return _map->getElevatorState( getElevatorId( ), idx );
}

ELEVATOR_POS Character::getActiveElevator( ) const {
	Vector position = _pos;
	position.y -= 1; //ぴったりになってしまうのを防ぐ
	int idx = ( int )( position.x / BLOCK_SIZE ) + ( int )( position.y / BLOCK_SIZE ) * _map->getCol( );
	return _map->getActiveElevator( getElevatorId( ), idx );
}

ELEVATOR_POS  Character::getDestination( ) const {
	Vector position = _pos;
	position.y -= 1; //ぴったりになってしまうのを防ぐ
	int idx = ( int )( position.x / BLOCK_SIZE ) + ( int )( position.y / BLOCK_SIZE ) * _map->getCol( );
	return _map->getDestination( getElevatorId( ), idx );
}

void Character::draw( ) {
	if ( !_draw_flag ) {
		return;
	}

	Vector chara_size = Vector( _anim[ _anim_type ].width, _anim[ _anim_type ].height );

	{ // キャラの描画
		if ( _anim.find( _anim_type ) == _anim.end( ) ) {
			return;
		}

		// カウントを進める
		_anim_cnt = ( _anim_cnt + 1 ) % _max_cnt;

		if ( _anim_cnt % _anim_change_time == 0 ) {
			// 画像を切り替える( 左端をずらす )
			_sx = ( ( _anim_cnt / _anim_change_time ) % _anim[ _anim_type ].frame ) * _anim[ _anim_type ].width;
		}

		bool flip = ( _move_dir == MOVE_DIR_RIGHT ? true : false );
		Vector pos = Vector( _pos.x - _anim[ _anim_type ].width / 2, _pos.y - _anim[ _anim_type ].height );
		_drawer->drawRectGraph( 
			( float )( pos.x + _scroll.x ), ( float )( pos.y + _scroll.y ),
			_sx, 0,
			_anim[ _anim_type ].width, _anim[ _anim_type ].height, 
			_anim[ _anim_type ].handle,
			true, flip );
	}

	{ // 情報の表示
		if ( _show_info_num < 1 ) {
			return;
		}

		Vector info = _pos - ( Vector( -chara_size.x * 0.25, chara_size.y ) ) + _scroll;

		int handle = _drawer->getImage( INFO_IMAGE );
		int image_height = _drawer->getImageHeight( INFO_IMAGE );
		const int IMAGE_GAP_Y = image_height - 30;
		const float FONT_GAP_X  = _drawer->getImageWidth( INFO_IMAGE ) * 0.3f;
		const float FONT_GAP_Y  = image_height * 0.25f;
		_drawer->setGraphAlpha( 100 );
		for ( int i = 0; i < _show_info_num; i++ ) {
			_drawer->drawGraph ( ( float )info.x, ( float )info.y + i * IMAGE_GAP_Y             , handle, true );
			_drawer->drawString( ( float )info.x + FONT_GAP_X, ( float )info.y + i * IMAGE_GAP_Y + FONT_GAP_Y, _info[ i ].c_str( ), BLUE );
		}
		_drawer->setGraphAlpha( 0 );
	}
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

void Character::setElevatorPos( char id ) {
	Vector pos = _map->getElevatorPos( id );
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