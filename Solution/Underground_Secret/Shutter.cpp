#include "Shutter.h"
#include "Drawer.h"
#include "Mouse.h"
#include "const.h"

const int MOVECOUNT_MAX = 60;

Shutter::Shutter( ) :
_col( 1 ) {
	_drawer = Drawer::getTask( );
	_mouse = Mouse::getTask( );

	_shutter_handle = _drawer->getImage( "shutter" );
	_shutter_width = _drawer->getImageWidth( "shutter" );
	_shutter_height = _drawer->getImageHeight( "shutter" );

	_scroll = Vector( );
}

Shutter::~Shutter( ) {
}

void Shutter::update( ) {
	onShutter( );
	calcShutter( );
}

void Shutter::draw( ) const {
	const int SHUTTER_MAX = ( int )_shutter.size( );
	const int MIN = BLOCK_SIZE;
	const int RATE = ( _shutter_height - MIN ) / MOVECOUNT_MAX;
	for ( int i = 0; i < SHUTTER_MAX; i++ ) {
		int ry = 0;
		int ly = 0;

		SHUTTER_STATE state = _shutter_state[ i ];

		switch ( state ) {
			case SHUTTER_STATE_ACTIVE: 
				ry = _shutter_height;
				break;

			case SHUTTER_STATE_NONACTIVE: 
				ry = MIN;
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

 		float x = ( float )( _shutter[ i ].front( ) % _col ) * BLOCK_SIZE;
		float y = ( float )( ( _shutter[ i ].front( ) / _col ) * BLOCK_SIZE );

		// スクロール分を足す
		x += ( float )_scroll.x * BLOCK_SIZE;
		y += ( float )_scroll.y * BLOCK_SIZE;

		_drawer->drawRectGraph( x, y, 0, ly, _shutter_width, ry, _shutter_handle, true );
	}
}

void Shutter::onShutter( ) {
	if ( !_mouse->isClickLeft( ) ) {
		return;
	}

	int mouse_x = _mouse->getPointX( );
	int mouse_y = _mouse->getPointY( );
	int idx = ( mouse_x / BLOCK_SIZE ) + ( ( mouse_y / BLOCK_SIZE ) * _col );
	idx += ( int )( _scroll.x * -1 ) + ( int )( ( _scroll.y * -1 ) * _col );

	int size = ( int )_shutter.size( );
	int hit_idx = -1;

	// シャッターの一番上のマスをクリックしているかどうか
	for ( int i = 0; i < size; i++ ) {
		if ( _shutter[ i ].front( ) != idx ) {
			continue;
		}
		hit_idx = i;
		break;
	}

	if ( hit_idx < 0 ) {
		return;
	}

	SHUTTER_STATE state = _shutter_state[ hit_idx ];

	// 閉まりきっているか、空ききっていない場合は処理しない
	if ( state != SHUTTER_STATE_NONACTIVE &&
		 state != SHUTTER_STATE_ACTIVE ) {
		return;
	}

	switch ( state ) {
	case SHUTTER_STATE_ACTIVE   : _shutter_state[ hit_idx ] = SHUTTER_STATE_OPEN ; break;
	case SHUTTER_STATE_NONACTIVE: _shutter_state[ hit_idx ] = SHUTTER_STATE_CLOSE; break;
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
	case SHUTTER_STATE_OPEN : _shutter_state[ action_idx ] = SHUTTER_STATE_NONACTIVE; break;
	case SHUTTER_STATE_CLOSE: _shutter_state[ action_idx ] = SHUTTER_STATE_ACTIVE   ; break;
	}
	_move_cnt = 0;
}

void Shutter::setCol( const int col ) {
	_col = col;
}

void Shutter::setScroll( Vector scroll ) {
	_scroll = scroll;
}

void Shutter::addShutter( std::vector< int > shutter ) {
	_shutter.push_back( shutter );
	_shutter_state.push_back( SHUTTER_STATE_NONACTIVE );
}

bool Shutter::isHitShutter( int detection_idx ) const {
	int action_idx = -1;

	// アクティブなシャッターを探す
	for ( int i = 0; i < ( int )_shutter_state.size( ); i++ ) {
		if ( _shutter_state[ i ] != SHUTTER_STATE_ACTIVE ) {
			continue;
		}
		action_idx = i;
		break;
	}

	if ( action_idx < 0 ) {
		return false;
	}

	// アクティブなシャッターのmapidxを比較する
	for ( int i = 0; i < _shutter[ action_idx ].size( ); i++ ) {
		if ( detection_idx == _shutter[ action_idx ][ i ] ) {
			return true;
		}
	}

	return false;
}