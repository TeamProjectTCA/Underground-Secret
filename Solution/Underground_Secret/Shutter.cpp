#include "Shutter.h"
#include "Drawer.h"
#include "Mouse.h"
#include "map.h"
#include "const.h"
#include <array>

const int DEFAULT_ON_SHUTTER_MAX = 2;
const int STAGE_ON_SHUTTER_NUM[ ] = { DEFAULT_ON_SHUTTER_MAX };

const int MOVECOUNT_MAX = 60;

Shutter::Shutter( int stage ) :
_col( 1 ),
ON_SHUTTER_MAX( STAGE_ON_SHUTTER_NUM[ stage - 1 ] ),
_active_num( 0 ) {
	_drawer = Drawer::getTask( );
	_mouse = Mouse::getTask( );

	_shutter_handle = _drawer->getImage( "shutter" );
	_shutter_width = _drawer->getImageWidth( "shutter" );
	_shutter_height = _drawer->getImageHeight( "shutter" );

	_scroll = Vector( );
	_end_scroll = Vector( );

	
}

Shutter::~Shutter( ) {
}

void Shutter::update( ) {
	onShutter( );
	calcShutter( );
}

void Shutter::draw( ) {
	const int SHUTTER_MAX = ( int )_shutter.size( );
	const int MIN = BLOCK_SIZE;
	const int RATE = ( _shutter_height - MIN ) / MOVECOUNT_MAX;
	const int SHUTTER_MARGIN_RIGHT = 10;
	const int SHUTTER_MARGIN_UP = 10;
	const int SWITCH_WIDTH = 16;
	const int SWITCH_HEIGHT = 16;


	for ( int i = 0; i < SHUTTER_MAX; i++ ) {
		int ry = 0;
		int ly = 0;

		SHUTTER_STATE state = _shutter_state[ i ];

		switch ( state ) {
			case SHUTTER_STATE_ACTIVE: 
				ry = _shutter_height;
				_shutter_switch_color[ i ] = RED;
				break;

			case SHUTTER_STATE_NONACTIVE: 
				ry = MIN;
				_shutter_switch_color[ i ] = GREEN;
				break;

			case SHUTTER_STATE_OPEN:
				ry = ( MOVECOUNT_MAX - _move_cnt ) * RATE + MIN;
				break;

			case SHUTTER_STATE_CLOSE:
				ry = _move_cnt * RATE + MIN;
				break;
		}

		// 描画はじめを設定
		ly = _shutter_height - ry;

 		float x = ( float )( ( _shutter[ i ].front( ) % _col ) * BLOCK_SIZE );
		float y = ( float )( ( _shutter[ i ].front( ) / _col ) * BLOCK_SIZE );

		// スクロール分を足す
		x += ( float )( _scroll.x - _end_scroll.x );
		y += ( float )( _scroll.y - _end_scroll.y );

		float switch_x = x + _shutter_width + SHUTTER_MARGIN_RIGHT;
		float switch_y = y - SHUTTER_MARGIN_UP - SWITCH_HEIGHT;

		_drawer->drawRectGraph( x, y, 0, ly, _shutter_width, ry, _shutter_handle, true );
		_drawer->drawBox( switch_x, switch_y, switch_x + SWITCH_WIDTH, switch_y + SWITCH_HEIGHT, _shutter_switch_color[ i ], true );
	}
}

void Shutter::onShutter( ) {
	if ( !_mouse->isClickDownLeft( ) ) {
		return;
	}

	int mouse_x = _mouse->getPointX( );
	int mouse_y = _mouse->getPointY( );

	int size = ( int )_shutter.size( );
	int hit_idx = -1;

	// シャッターの一番上のマスをクリックしているかどうか
	for ( int i = 0; i < size; i++ ) {
		int x = ( _shutter[ i ].front( ) % _col ) * BLOCK_SIZE;
		int y = ( _shutter[ i ].front( ) / _col ) * BLOCK_SIZE;
		x += ( int )_scroll.x;
		y += ( int )_scroll.y ;

		if ( mouse_x < x || mouse_x > ( x + BLOCK_SIZE ) ||
			 mouse_y < y || mouse_y > ( y + BLOCK_SIZE ) ) {
			continue;
		}

		hit_idx = i;
		break;
	}

	if ( hit_idx < 0 ) {
		return;
	}

	for ( int i = 0; i < size; i++ ) {
	// 閉まりきっているか、空ききっていない場合は処理しない
		if ( _shutter_state[ i ] != SHUTTER_STATE_NONACTIVE &&
		     _shutter_state[ i ] != SHUTTER_STATE_ACTIVE ) {
			return;
		}
	}

	SHUTTER_STATE state = _shutter_state[ hit_idx ];

	switch ( state ) {
	case SHUTTER_STATE_ACTIVE   :
		_shutter_state[ hit_idx ] = SHUTTER_STATE_OPEN;
		break;

	case SHUTTER_STATE_NONACTIVE:
		if ( _active_num == ON_SHUTTER_MAX ) {
			return;
		}
		_shutter_state[ hit_idx ] = SHUTTER_STATE_CLOSE;
		_active_num++;
		break;
	}
}

void Shutter::calcShutter( ) {
	int size = ( int )_shutter_state.size( );
	bool detection = false;
	int action_idx = -1;

	for ( int i = 0; i < size; i++ ) {
		if ( _shutter_state[ i ] != SHUTTER_STATE_OPEN &&
			 _shutter_state[ i ] != SHUTTER_STATE_CLOSE ) {
			continue;
		}
		action_idx = i;
		detection = true;
		break;
	}

	if ( !detection ) {
		return;
	}

	if ( _move_cnt < MOVECOUNT_MAX ) {
		_move_cnt++;
		return;
	}

	// 完全にカウントが満たしたらステータスを変更
	switch ( _shutter_state[ action_idx ] ) {
	case SHUTTER_STATE_OPEN : 
		_shutter_state[ action_idx ] = SHUTTER_STATE_NONACTIVE;
		_active_num--;
		break;

	case SHUTTER_STATE_CLOSE: 
		_shutter_state[ action_idx ] = SHUTTER_STATE_ACTIVE;
		break;
	}
	_move_cnt = 0;
}

void Shutter::setCol( const int col ) {
	_col = col;
}

void Shutter::setScroll( Vector scroll ) {
	_scroll = scroll;
}

void Shutter::setEndScroll( Vector end_scroll ) {
	_end_scroll = end_scroll;
}

void Shutter::addShutter( std::vector< int > shutter ) {
	_shutter.push_back( shutter );
	_shutter_state.push_back( SHUTTER_STATE_NONACTIVE );
	_shutter_switch_color.push_back( GREEN );
}

bool Shutter::isHitShutter( int detection_idx ) const {

	int size = ( int )_shutter_state.size( );
	std::vector< int > active_idx;

	// アクティブなものを探す
	for ( int i = 0; i < size; i++ ) {
		if ( _shutter_state[ i ] != SHUTTER_STATE_ACTIVE ) {
			continue;
		}
		active_idx.push_back( i );
	}

	// 判定する
	size = ( int )active_idx.size( );
	for ( int i = 0; i < size; i++ ) {
		int shutter_last_idx = ( int )_shutter[ active_idx[ i ] ].size( ) - 1;
		if ( _shutter[ active_idx[ i ] ][ shutter_last_idx ] == detection_idx ) {
			return true;
		}
	}

	return false;
}