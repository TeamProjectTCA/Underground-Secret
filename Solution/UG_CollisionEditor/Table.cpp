#include "Table.h"
#include "Mouse.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "const.h"

const int SCROLL_NUM = 2;

Table::Table( std::string file_name ) {
	_mouse = Mouse::getTask( );
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	
	_handle = _drawer->getImage( file_name );
	//縦横にそれぞれ1マス分バッファを持たせる
	_col = _drawer->getImageWidth( file_name ) / BLOCK_SIZE + 1;
	_row = _drawer->getImageHeight( file_name ) / BLOCK_SIZE + 1;
	_command = COMMAND_PUT;
	_size = 1;
	
	if ( file_name == "" ) {
		_col = 1;
		_row = 1;
	}

	unsigned int length = _col * _row + 1;
	_data = ( char* )malloc( sizeof( char ) * length );
	memset( _data, 0, sizeof( char ) * length );
}

Table::~Table( ) {
}

void Table::update( ) {
	if( _handle < 0 ) {
		return;
	}
	//当たり判定の範囲を設定
	setRange( );

	//スクロールの計算
	scroll( );

	//コマンド選択
	setCommand( );

	//設置
	setCollider( );

	//描画
	_drawer->flip( );

	drawLoadedImage( );
	drawActiveCollider( );
	drawTableSelect( );
	drawTable( );

	_drawer->drawFormatString( 20, 20, 0xffffff, "  x : %d,   y: %d", _x, _y );
	_drawer->drawFormatString( 20, 40, 0xffffff, "col : %d, row: %d", _col, _row );
	_drawer->drawFormatString( 20, 60, 0xffffff, "idx : %d"         , _idx );
}

void Table::memoryFree( ) {
	free( _data );
}

void Table::scroll( ) {
	//左
	if ( _keyboard->getKeyDown( "a" ) || _keyboard->getState( "a" ) > 30 ) {
		_x -= SCROLL_NUM;
	}
	//右
	if ( _keyboard->getKeyDown( "d" ) || _keyboard->getState( "d" ) > 30 ) {
		_x += SCROLL_NUM;
	}
	//上
	if ( _keyboard->getKeyDown( "w" ) || _keyboard->getState( "w" ) > 30 ) {
		_y -= SCROLL_NUM;
	}
	//下
	if ( _keyboard->getKeyDown( "s" ) || _keyboard->getState( "s" ) > 30 ) {
		_y += SCROLL_NUM;
	}


	if ( _x > 0 ) {
		_x = 0;
	}
	if ( _x < WIDTH / BLOCK_SIZE - ( _col ) ) {
		_x =  WIDTH / BLOCK_SIZE - ( _col );
	}

	if ( _y > 0 ) {
		_y = 0;
	}
	if ( _y < HEIGHT / BLOCK_SIZE - ( _row ) ) {
		_y =  HEIGHT / BLOCK_SIZE - ( _row );
	}
}

void Table::setCommand( ) {
	if ( !_mouse->isClickRight( ) ) {
		return;
	}

	_command = ( COMMAND )( ( _command + 1 ) % COMMAND_MAX );
}

void Table::setCollider( ) {
	int mouse_x = _mouse->getPointX( );
	int mouse_y = _mouse->getPointY( );

	mouse_x /= BLOCK_SIZE;
	mouse_y /= BLOCK_SIZE;

	if ( mouse_x >= _col + _x || mouse_y >= _row + _y ||
		 mouse_x <     0 + _x || mouse_y <     0 + _y ) {
		_idx = -1;
		return;
	}

	_idx = ( mouse_x - _x ) + ( mouse_y - _y ) * _col;

	if ( _mouse->getClickingLeft( ) ) {
		switch ( _command ) {

		//判定取り消し
		case COMMAND_DELETE:
		{
			//左上にセット
			int gap = 0;
			if ( _idx % _col - _size / 2 < 0 ) {
				//左に寄りすぎて超過した分を検出
				gap = ( int )abs( _idx % _col - _size / 2 );
			}

			long long int x = ( _idx + gap - _size / 2 ) % _col;
			long long int y = ( _idx - ( _size / 2 ) * _col ) / _col;
			long long int idx = x + y * _col;

			//右にsize分,下にsize分の四角をすべて0にする
			for ( int i = 0; i < _size; i++ ) {
				for ( int j = 0; j < _size - gap; j++ ) {
					//右端を過ぎたら
					if ( ( idx + j + i * _col ) / _col > y + i ||
						 ( idx + j + i * _col ) / _col < y + i ) {
						continue;
					}
					long long int del = idx + i * _col + j;

					//縦と横の超過を検出
					if ( del < 0 || ( _col * _row ) - 1 < del ) {
						continue;
					}
					_data[ del ] = 0;
				}
			}
		}
		break;

		//当たり判定付与
		case COMMAND_PUT:
		{
			//左上にセット
			int gap = 0;
			if ( _idx % _col - _size / 2 < 0 ) {
				//左に寄りすぎて超過した分を検出
				gap = ( int )abs( _idx % _col - _size / 2 );
			}

			long long int x = ( _idx + gap - _size / 2 ) % _col;
			long long int y = ( _idx - ( _size / 2 ) * _col ) / _col;
			long long int idx = x + y * _col;

			//右にsize分,下にsize分の四角をすべて0にする
			for ( int i = 0; i < _size; i++ ) {
				for ( int j = 0; j < _size - gap; j++ ) {
					//右端を過ぎたら
					if ( ( idx + j + i * _col ) / _col > y + i ||
						 ( idx + j + i * _col ) / _col < y + i ) {
						continue;
					}
					long long int put = idx + i * _col + j;

					//縦と横の超過を検出
					if ( put < 0 || ( _col * _row ) - 1 < put ) {
						continue;
					}
					_data[ put ] = 1;
				}
			}
		}
		break;

		default:
			break;
		}
	}
}

void Table::setRange( ) {
	std::string key = _keyboard->getString( );
	char c_num = key[ 0 ];
	if( c_num <= '9' && c_num >= '0' ) {
		_size = c_num - '0';
	}
}

void Table::drawLoadedImage( ) const{
	_drawer->drawGraph( _x * BLOCK_SIZE, _y * BLOCK_SIZE, _handle, true );
}

void Table::drawTable( ) const {
	for ( int i = 1; i < HEIGHT / BLOCK_SIZE; i++ ) {
		_drawer->drawLine( ( float )0, ( float )i * BLOCK_SIZE, ( float )WIDTH, ( float )i * BLOCK_SIZE, 0x3b3b3b );
	}
	for ( int i = 1; i < WIDTH / BLOCK_SIZE; i++ ) {
		_drawer->drawLine( ( float )i * BLOCK_SIZE, ( float )0, ( float )i * BLOCK_SIZE, ( float )HEIGHT, 0x3b3b3b );
	}
}

void Table::drawTableSelect( ) const {
	if ( _idx < 0 ) {
		return;
	}

	//左上にセット
	int gap = 0;
	if ( _idx % _col - _size / 2 < 0 ) {
		//左に寄りすぎて超過した分を検出
		gap = ( int )abs( _idx % _col - _size / 2 );
	}
	long long int x = ( _idx + gap - _size / 2 ) % _col;
	long long int y = ( _idx - ( _size / 2 ) * _col ) / _col;
	long long int idx = x + y * _col;

	unsigned int color = ( _command == COMMAND_DELETE ? 0x5555ff : 0xff5555 );

	for ( int i = 0; i < _size; i++ ) {
		for ( int j = 0; j < _size - gap; j++ ) {

			_drawer->drawBox( ( float )(     x + _x + j ) * BLOCK_SIZE, ( float )(     y + _y + i ) * BLOCK_SIZE,
							  ( float )( x + 1 + _x + j ) * BLOCK_SIZE, ( float )( y + 1 + _y + i ) * BLOCK_SIZE,
							  color, true );
		}
	}
}

void Table::drawActiveCollider( ) const {
	for ( int i = 0; i < _row; i++ ) {
		for ( int j = 0; j < _col; j++ ) {
			int idx = i * _col + j;
			if ( !_data[ idx ] ) {
				continue;
			}

			int x = idx % _col;
			int y = idx / _col;

			_drawer->drawBox( ( float )(     x + _x ) * BLOCK_SIZE, ( float )(     y + _y ) * BLOCK_SIZE, 
				              ( float )( x + 1 + _x ) * BLOCK_SIZE, ( float )( y + 1 + _y ) * BLOCK_SIZE,
				              0xff0000, true );
		}
	}
}