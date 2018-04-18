#pragma once
#include "smart_ptr.h"
#include <vector>
#include <string>

PTR( Table );
PTR( Mouse );
PTR( Drawer );
PTR( Keyboard );

class Table {
private:
	enum COMMAND {
		COMMAND_DELETE,
		COMMAND_PUT,
		COMMAND_MAX
	};

public:
	Table( std::string file_name );
	virtual ~Table( );

public:
	void update( );
	void memoryFree( );

private:
	void scroll( );
	void setCommand( );
	void setCollider( );
	void setRange( );

private:
	void drawLoadedImage( ) const;
	void drawTable( ) const;
	void drawTableSelect( ) const; 
	void drawActiveCollider( ) const;

private:
	//左上の配列番号
	int _x;
	int _y;

	//画像に対しての縦横
	int _col;
	int _row;

	//その他
	long long int _idx;
	int _handle;
	int _size;

	//コライダーデータ
	char *_data;


	COMMAND _command;

	MousePtr _mouse;
	DrawerPtr _drawer;
	KeyboardPtr _keyboard;
};

