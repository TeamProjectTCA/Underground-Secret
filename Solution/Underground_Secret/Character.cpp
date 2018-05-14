#include "Character.h"
#include "Drawer.h"
#include "Map.h"
#include "const.h"
#include <assert.h>
#include <errno.h>

const int DEFAULT_ANIM_TIME = 30;
const int DEFAULT_MAX_COUNT = 0xffffff;

Character::Character( MapPtr map ) :
_map( map ) {
	_drawer = Drawer::getTask( );

	_anim_type = WALK;
	_anim_change_time = DEFAULT_ANIM_TIME;

	_cnt = 0;
	_sx = 0;
	_max_cnt = DEFAULT_MAX_COUNT;
	_pos = Vector( );
	_scroll = Vector( );
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

Vector Character::getPos( ) const {
	return _pos;
}

int Character::getMapData( Vector pos ) const {
	int data = -1;

	Vector position = pos;
	position.x += ( int )_anim.find( _anim_type )->second.width / 2;
	position.y += ( int )_anim.find( _anim_type )->second.height - 5;

	int idx = ( int )( position.x / BLOCK_SIZE ) + ( int )( position.y / BLOCK_SIZE ) * _map->getCol( );

	data = _map->getMapData( idx );

	if ( 'a' <= data && data <= 'z' ) {
		data -= 'a';
	}
	if ( '0' <= data && data <= '9' ) {
		data -= '0';
	}

	// debug
	_drawer->drawBox( 
		( float )( _pos.x + _scroll.x * BLOCK_SIZE ), 
		( float )( _pos.y + _scroll.y * BLOCK_SIZE ), 
		( float )( position.x + _scroll.x * BLOCK_SIZE ), 
		( float )( position.y + _scroll.y * BLOCK_SIZE ), 
		RED, false );

	_drawer->drawBox( 
		( float )( idx % _map->getCol( ) + _scroll.x ) * BLOCK_SIZE, 
		( float )( idx / _map->getCol( ) + _scroll.y ) * BLOCK_SIZE,
		( float )( idx % _map->getCol( ) + _scroll.x + 1 ) * BLOCK_SIZE, 
		( float )( idx / _map->getCol( ) + _scroll.y + 1 ) * BLOCK_SIZE,
		BLUE, true );

	return data;
}

void Character::draw( ) {
	if ( _anim.find( _anim_type ) == _anim.end( ) ) {
		return;
	}
	
	_scroll = _map->getScrollData( );
	_cnt = ( _cnt + 1 ) % _max_cnt;

	if ( _cnt % _anim_change_time == 0 ) {
		// ‰æ‘œ‚ðØ‚è‘Ö‚¦‚é( ¶’[‚ð‚¸‚ç‚· )
		_sx = ( ( _cnt / _anim_change_time ) % _anim[ _anim_type ].frame ) * _anim[ _anim_type ].width;
	}

	_drawer->drawRectGraph( 
		( float )( _pos.x + _scroll.x * BLOCK_SIZE ), ( float )( _pos.y + _scroll.y * BLOCK_SIZE ),
		_sx, 0,
		_anim[ _anim_type ].width, _anim[ _anim_type ].height, 
		_anim[ _anim_type ].handle,
		true, false );
}

void Character::setPos( Vector pos ) {
	_pos = pos;
}