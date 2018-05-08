#include "Table.h"
#include "Mouse.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "const.h"

const int SCROLL_NUM = 2;
const int COMMAND_ICON_SIZE = 64;
const int COMMAND_ICON_NUM = 6; // 1ページに表示するアイコン数
const int COMMAND_ICON_START_X = 30;
const int COMMAND_ICON_PITCH = 15;
const int PAGE_CHANGE_ICON_SIZE = 30;
const int PAGE_CHANGE_ICON_PITCH = 30;
const int NOT_DOOR_COMMAND_NUM = 6;

Table::Table( const int handle, const int col, const int row ) :
_map_handle( handle ),
// 縦横にそれぞれ1マス分バッファを持たせる
_col( col + 1 ),
_row( row + 1 ) {
	_mouse = Mouse::getTask( );
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	
	_command_select = false;
	_command = SET;
	_size = 1;
	_menu_x = 0;
	_menu_y = 0;
	_page_change_handle = _drawer->getImage( "page_change" );
	_menu_handle = _drawer->getImage( "menu" );
	_menu_width  = _drawer->getImageWidth( "menu" );
	_menu_height = _drawer->getImageHeight( "menu" );
	_command_handle[ A ] = _drawer->getImage( "command_door_a" );
	_command_handle[ B ] = _drawer->getImage( "command_door_b" );
	_command_handle[ C ] = _drawer->getImage( "command_door_c" );
	_command_handle[ D ] = _drawer->getImage( "command_door_d" );
	_command_handle[ E ] = _drawer->getImage( "command_door_e" );
	_command_handle[ F ] = _drawer->getImage( "command_door_f" );
	_command_handle[ G ] = _drawer->getImage( "command_door_g" );
	_command_handle[ H ] = _drawer->getImage( "command_door_h" );
	_command_handle[ I ] = _drawer->getImage( "command_door_i" );
	_command_handle[ J ] = _drawer->getImage( "command_door_j" );
	_command_handle[ K ] = _drawer->getImage( "command_door_k" );
	_command_handle[ L ] = _drawer->getImage( "command_door_l" );
	_command_handle[ M ] = _drawer->getImage( "command_door_m" );
	_command_handle[ N ] = _drawer->getImage( "command_door_n" );
	_command_handle[ O ] = _drawer->getImage( "command_door_o" );
	_command_handle[ P ] = _drawer->getImage( "command_door_p" );
	_command_handle[ SET ] = _drawer->getImage( "command_set" );
	_command_handle[ DEL ] = _drawer->getImage( "command_del" );
	_command_handle[ SHUTTER ] = _drawer->getImage( "command_shutter" );
	_command_handle[ FP_1 ] = _drawer->getImage( "command_fp1" );
	_command_handle[ FP_2 ] = _drawer->getImage( "command_fp2" );
	_command_handle[ FP_3 ] = _drawer->getImage( "command_fp3" );

	unsigned int length = _col * _row + 1;
	_data = ( char* )malloc( sizeof( char ) * length );
	for ( unsigned int i = 0; i < length; i++ ) {
		_data[ i ] = '0';
	}
	_data[ length - 1 ] = '\0';
}

Table::~Table( ) {
	memoryFree( );
}

void Table::update( ) {
	if( _map_handle < 0 ) {
		return;
	}

	if ( !_command_select ) {
		// 当たり判定の範囲を設定
		setRange( );

		// スクロールの計算
		scroll( );

		// 設置
		setCommand( );
	} else {
		selectCommand( );
	}
	// コマンド選択
	showCommandMenu( );

	// 描画
	drawLoadedImage( );
	drawActiveCollider( );
	drawTableSelect( );
	drawTable( );
	drawCommandMenu( );

	_drawer->drawFormatString( 20, 20, RED, "x : %d, y : %d", _x, _y );
	_drawer->drawFormatString( 20, 40, RED, "col : %d, row : %d", _col, _row );
	_drawer->drawFormatString( 20, 60, RED, "idx : %d", _idx );
	_drawer->drawFormatString( 20, 80, RED, "command : %d", _command );

	_drawer->flip( );
}

void Table::memoryFree( ) {
	if ( _data != NULL ) {
		free( _data );
		_data = NULL;
	}
}

char* Table::getData( ) {
	return _data;
}

int Table::getCol( ) const {
	return _col;
}

int Table::getRow( ) const {
	return _row;
}

void Table::loadCollider( std::string data, const int col, const int row ) {
	_col = col;
	_row = row;

	unsigned int length = _col * _row + 1;
	_data = ( char* )malloc( sizeof( char ) * length );
	for ( unsigned int i = 0; i < length; i++ ) {
		_data[ i ] = data[ i ];
	}

	_data[ length - 1 ] = '\0';
}

void Table::scroll( ) {
	//左
	if ( _keyboard->getKeyDown( "d" ) || _keyboard->getState( "d" ) > 30 ) {
		_x -= SCROLL_NUM;
	}
	//右
	if ( _keyboard->getKeyDown( "a" ) || _keyboard->getState( "a" ) > 30 ) {
		_x += SCROLL_NUM;
	}
	//上
	if ( _keyboard->getKeyDown( "s" ) || _keyboard->getState( "s" ) > 30 ) {
		_y -= SCROLL_NUM;
	}
	//下
	if ( _keyboard->getKeyDown( "w" ) || _keyboard->getState( "w" ) > 30 ) {
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

void Table::showCommandMenu( ) {
	if ( !_mouse->isClickRight( ) ) {
		return;
	}

	// コマンドメニュー表示の切り替え
	_command_select = !_command_select;
	_menu_x = _mouse->getPointX( );
	_menu_y = _mouse->getPointY( );

	if ( !_command_select ) {
		return;
	}
}

void Table::setCommand( ) {
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

	if ( !_mouse->getClickingLeft( ) ) {
		return;
	}

	if ( _command >= COMMAND_MAX ) {
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

			_data[ put ] = convCommandToExportStr( _command ).front( );
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

void Table::selectCommand( ) {
	if ( !_mouse->isClickUpLeft( ) ) {
		return;
	}
	// ページ切り替え
	changePage( );

	// コマンド切り替え
	changeCommand( );
}

void Table::changePage( ) {
	int mouse_x = _mouse->getPointX( );
	int mouse_y = _mouse->getPointY( );

	// 描画が中心座標なのでそれぞれ半分ずつ足し引きする
	int page_back_x = _menu_x + PAGE_CHANGE_ICON_SIZE / 2;
	int page_next_x = _menu_x + _menu_width - ( int )( PAGE_CHANGE_ICON_PITCH * 1.5 );
	int page_change_y = _menu_y + _menu_height - ( int )( PAGE_CHANGE_ICON_PITCH * 1.5 );

	const int PAGE_MAX = ( int )_command_handle.size( ) / COMMAND_ICON_NUM + 1;
	int next = 0;

	// 戻る
	if ( page_back_x   <= mouse_x && mouse_x <= page_back_x   + PAGE_CHANGE_ICON_SIZE &&
		 page_change_y <= mouse_y && mouse_y <= page_change_y + PAGE_CHANGE_ICON_SIZE ) {
		next = PAGE_MAX - 1;
	}

	// 進む
	if ( page_next_x   <= mouse_x && mouse_x <= page_next_x   + PAGE_CHANGE_ICON_SIZE &&
		 page_change_y <= mouse_y && mouse_y <= page_change_y + PAGE_CHANGE_ICON_SIZE ) {
		next = 1;
	}

	// ページを切り替える
	_menu_page = ( _menu_page + next ) % PAGE_MAX;
}

void Table::changeCommand( ) {
	int mouse_x = _mouse->getPointX( );
	int mouse_y = _mouse->getPointY( );
	int icon_x = _menu_x + COMMAND_ICON_START_X;
	int icon_y = _menu_y + COMMAND_ICON_PITCH;

	int hit = -1;
	for ( int i = 0; i < COMMAND_ICON_NUM; i++ ) {
		int gap_x = ( i % ( COMMAND_ICON_NUM / 2 ) ) * ( COMMAND_ICON_SIZE + COMMAND_ICON_PITCH );
		int gap_y = ( i / ( COMMAND_ICON_NUM / 2 ) ) * ( COMMAND_ICON_SIZE + COMMAND_ICON_PITCH );

		if ( icon_x + gap_x <= mouse_x && mouse_x <= icon_x + gap_x + COMMAND_ICON_SIZE &&
			 icon_y + gap_y <= mouse_y && mouse_y <= icon_y + gap_y + COMMAND_ICON_SIZE ) {
			hit = i;
			break;
		}
	}

	if ( hit != -1 ) {
		_command = ( COMMAND )( hit + _menu_page * COMMAND_ICON_NUM );
		_command_select = false;
	}
}

std::string Table::convCommandToExportStr( COMMAND command ) {
	std::string str = "";

	switch ( command ) {
	case SET     : str = "1"; break;
	case DEL     : str = "0"; break;
	case SHUTTER : str = "2"; break;
	case FP_1    : str = "3"; break;
	case FP_2    : str = "4"; break;
	case FP_3    : str = "5"; break;
	case A       : str = "a"; break;
	case B       : str = "b"; break;
	case C       : str = "c"; break;
	case D       : str = "d"; break;
	case E       : str = "e"; break;
	case F       : str = "f"; break;
	case G       : str = "g"; break;
	case H       : str = "h"; break;
	case I       : str = "i"; break;
	case J       : str = "j"; break;
	case K       : str = "k"; break;
	case L       : str = "l"; break;
	case M       : str = "m"; break;
	case N       : str = "n"; break;
	case O       : str = "o"; break;
	case P       : str = "p"; break;
	default : break;
	}

	return str;
}

void Table::drawLoadedImage( ) const{
	_drawer->drawGraph( _x * BLOCK_SIZE, _y * BLOCK_SIZE, _map_handle, true );
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

	unsigned int color = ( _command == DEL ? 0x5555ff : 0xff5555 );

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
			if ( _data[ idx ] == '0' ) {
				continue;
			}

			int x = idx % _col;
			int y = idx / _col;

			// 当たり判定
			if ( _data[ idx ] == '1' ) {
				_drawer->drawBox( ( float )(     x + _x ) * BLOCK_SIZE, ( float )(     y + _y ) * BLOCK_SIZE, 
					              ( float )( x + 1 + _x ) * BLOCK_SIZE, ( float )( y + 1 + _y ) * BLOCK_SIZE,
						          0xff0000, true );
			}

			// シャッター
			if ( _data[ idx ] == '2' ) {
					_drawer->drawRotaGraph( ( float )( x + _x ) * BLOCK_SIZE + BLOCK_SIZE / 2, ( float )( y + _y ) * BLOCK_SIZE + BLOCK_SIZE / 2,
						                0.25, 0, _command_handle[ SHUTTER ], true );
			}

			// Fixedpoint(定点)
			if ( _data[ idx ] == '3' || _data[ idx ] == '4' || _data[ idx ] == '5' ) {
				int handle_idx = _data[ idx ] - '0';
				_drawer->drawRotaGraph( ( float )( x + _x ) * BLOCK_SIZE + BLOCK_SIZE / 2, ( float )( y + _y ) * BLOCK_SIZE + BLOCK_SIZE / 2,
						                0.25, 0, _command_handle[ handle_idx ], true );
			}


			// エレベーター
			const int DOOR_MAX = COMMAND_MAX - NOT_DOOR_COMMAND_NUM;
			if ( _data[ idx ] >= 'a' && _data[ idx ] <= 'a' + DOOR_MAX ) {
				_drawer->drawRotaGraph( ( float )( x + _x ) * BLOCK_SIZE + BLOCK_SIZE / 2, ( float )( y + _y ) * BLOCK_SIZE + BLOCK_SIZE / 2,
						                0.25, 0, _command_handle[ _data[ idx ] - 'a' + NOT_DOOR_COMMAND_NUM ], true );
			}
		}
	}
}

void Table::drawCommandMenu( ) const {
	if ( !_command_select ) {
		return;
	}
	_drawer->drawGraph( _menu_x, _menu_y, _menu_handle, true );

	// ページ切り替えボタン
	_drawer->drawRotaGraph( 
		( float )_menu_x + PAGE_CHANGE_ICON_SIZE, 
		( float )( _menu_y + _menu_height - PAGE_CHANGE_ICON_SIZE ), 
		1, 0, _page_change_handle, true );

	_drawer->drawRotaGraph( 
		( float )( _menu_x + _menu_width - PAGE_CHANGE_ICON_SIZE ),
		( float )( _menu_y + _menu_height - PAGE_CHANGE_ICON_SIZE ),
		1, PI, _page_change_handle, true );

	int icon_x = _menu_x + COMMAND_ICON_START_X;
	int icon_y = _menu_y + COMMAND_ICON_PITCH;

	for ( int i = 0; i < COMMAND_ICON_NUM; i++ ) {
		if ( COMMAND_ICON_NUM * _menu_page + i >= _command_handle.size( ) ) {
			break;
		}

		int gap_x = ( i % ( COMMAND_ICON_NUM / 2 ) ) * ( COMMAND_ICON_SIZE + COMMAND_ICON_PITCH );
		int gap_y = ( i / ( COMMAND_ICON_NUM / 2 ) ) * ( COMMAND_ICON_SIZE + COMMAND_ICON_PITCH );
		
		_drawer->drawGraph( icon_x + gap_x, icon_y + gap_y, _command_handle[ i + _menu_page * COMMAND_ICON_NUM ], true );
	}
}