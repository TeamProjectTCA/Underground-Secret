#pragma once
#include "smart_ptr.h"
#include <vector>
#include <string>
#include <array>

PTR( Table );
PTR( Mouse );
PTR( Drawer );
PTR( Keyboard );

class Table {
private:
	enum COMMAND {
		SET,
		DEL,
		SHUTTER,
		FP_1,
		FP_2,
		FP_3,
		a,
		A,
		b,
		B,
		c,
		C,
		d,
		D,
		e,
		E,
		f,
		F,
		g,
		G,
		h,
		H,
		ENDPOINT,
		COMMAND_MAX
	};

public:
	Table( const int handle, const int col, const int row );
	virtual ~Table( );

public:
	void update( );
	void memoryFree( );

public:
	char* getData( );
	int getCol( ) const;
	int getRow( ) const;

public:
	void loadCollider( std::string data, const int col, const int row );

private:
	void scroll( );
	void showCommandMenu( );
	void setCommand( );
	void setRange( );
	void selectCommand( );
	void changePage( );
	void changeCommand( );
	std::string convCommandToExportStr( COMMAND command );

private:
	void drawLoadedImage( ) const;
	void drawTable( ) const;
	void drawTableSelect( ) const; 
	void drawActiveCollider( ) const;
	void drawCommandMenu( ) const;

private:
	//左上の配列番号
	int _x;
	int _y;

	//画像に対しての縦横
	int _col;
	int _row;

	//コライダーデータ
	char *_data;

	//その他
	long long int _idx;
	int _menu_x;
	int _menu_y;
	int _menu_width;
	int _menu_height;
	int _menu_handle;
	int _map_handle;
	int _size;
	int _page_change_handle;
	int _menu_page;
	bool _command_select;
	COMMAND _command;
	std::array< int, COMMAND_MAX > _command_handle;

	MousePtr _mouse;
	DrawerPtr _drawer;
	KeyboardPtr _keyboard;
};

