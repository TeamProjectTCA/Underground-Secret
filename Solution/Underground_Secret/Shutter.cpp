#include "Shutter.h"
#include "Drawer.h"
#include "Mouse.h"
#include "Button.h"
#include "map.h"
#include "const.h"
#include <array>

const int DEFAULT_ON_SHUTTER_MAX = 2;
const int SWITCH_WIDTH = 64;
const int SWITCH_HEIGHT = 32;
const int STAGE_ON_SHUTTER_NUM[ ] = { DEFAULT_ON_SHUTTER_MAX };
const char SHUTTER_OPEN_NORMAL_IMAGE[ ] = "button_on";
const char SHUTTER_OPEN_PUSH_IMAGE[ ] = "button_on";
const char SHUTTER_CLOSE_NORMAL_IMAGE[ ] = "button_off";
const char SHUTTER_CLOSE_PUSH_IMAGE[ ] = "button_off";

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
	const int SHUTTER_MARGIN_RIGHT = 5;
	const int SHUTTER_MARGIN_UP = 5;


	for ( int i = 0; i < SHUTTER_MAX; i++ ) {
		int ry = 0;
		int ly = 0;

		SHUTTER_STATE state = _shutter_state[ i ];

		switch ( state ) {
			case SHUTTER_STATE_ACTIVE:
				ry = _shutter_height;
				_button[ i ]->setImage( SHUTTER_OPEN_NORMAL_IMAGE );
				_button[ i ]->setPushImage( SHUTTER_OPEN_PUSH_IMAGE );
				break;

			case SHUTTER_STATE_NONACTIVE:
				ry = MIN;
				_button[ i ]->setImage( SHUTTER_CLOSE_NORMAL_IMAGE );
				_button[ i ]->setPushImage( SHUTTER_CLOSE_PUSH_IMAGE );
				break;

			case SHUTTER_STATE_OPEN:
				ry = ( MOVECOUNT_MAX - _move_cnt ) * RATE + MIN;
				_button[ i ]->setImage( SHUTTER_CLOSE_NORMAL_IMAGE );
				_button[ i ]->setPushImage( SHUTTER_CLOSE_PUSH_IMAGE );
				break;

			case SHUTTER_STATE_CLOSE:
				ry = _move_cnt * RATE + MIN;
				_button[ i ]->setImage( SHUTTER_CLOSE_NORMAL_IMAGE );
				_button[ i ]->setPushImage( SHUTTER_CLOSE_PUSH_IMAGE );
				break;
		}

		// 描画はじめを設定
		ly = _shutter_height - ry;

 		float x = ( float )( ( _shutter[ i ].front( ) % _col ) * BLOCK_SIZE );
		float y = ( float )( ( _shutter[ i ].front( ) / _col ) * BLOCK_SIZE );

		// スクロール分を足す
		x += ( float )( _scroll.x - _end_scroll.x );
		y += ( float )( _scroll.y - _end_scroll.y );

		float switch_x = x + _shutter_width / 2.0f;
		float switch_y = y - SHUTTER_MARGIN_UP - SWITCH_HEIGHT / 2.0f;
		Vector BUTTON_POSITION = Vector( switch_x, switch_y );

		_button[ i ]->setCollider( 
			( float )BUTTON_POSITION.x - SWITCH_WIDTH / 2.0f,
			( float )BUTTON_POSITION.y - SWITCH_HEIGHT / 2.0f,
			( float )BUTTON_POSITION.x + SWITCH_WIDTH / 2.0f,
			( float )BUTTON_POSITION.y + SWITCH_HEIGHT / 2.0f );
		_button[ i ]->setPos( 
			( float )BUTTON_POSITION.x - SWITCH_WIDTH / 2.0f,
			( float )BUTTON_POSITION.y - SWITCH_HEIGHT / 2.0f,
			( float )BUTTON_POSITION.x + SWITCH_WIDTH / 2.0f,
			( float )BUTTON_POSITION.y + SWITCH_HEIGHT / 2.0f );

		_drawer->drawRectGraph( x, y, 0, ly, _shutter_width, ry, _shutter_handle, true );
		_button[ i ]->draw( );

	}
}

void Shutter::onShutter( ) {
	int size = ( int )_shutter.size( );

	for ( int i = 0; i < size; i++ ) {
		// 閉まりきっているか、空ききっていない場合は処理しない
		if ( _shutter_state[ i ] != SHUTTER_STATE_NONACTIVE &&
			_shutter_state[ i ] != SHUTTER_STATE_ACTIVE ) {
			return;
		}
	}

	Vector mouse_pos = _mouse->getPoint( );
	int hit_idx = -1;
	
	//スイッチを使ってシャッターを起動
	for ( int i = 0; i < size; i++ ) {
		if ( _mouse->getClickingLeft( ) ) {
			_button[ i ]->click( mouse_pos );
		} else {
			if ( _button[ i ]->isPush( ) ) {
				hit_idx = i;
			}
			_button[ i ]->resetState( );
		}
	}
	

	if ( hit_idx < 0 ) {
		return;
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
	_button.push_back( ButtonPtr( new Button( ) ) );
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
	const int PITCH = 3;
	size = ( int )active_idx.size( );
	for ( int i = 0; i < size; i++ ) {
		int shutter_last_idx = ( int )_shutter[ active_idx[ i ] ].size( ) - 1;
		int idx = _shutter[ active_idx[ i ] ][ shutter_last_idx ];

		for ( int j = 0; j < PITCH; j++ ) {
			idx += j;
			if ( idx == detection_idx ) {
				return true;
			}
		}
	}

	return false;
}