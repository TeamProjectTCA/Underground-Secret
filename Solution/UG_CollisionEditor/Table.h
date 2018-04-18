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
	Table( const int handle, const int col, const int row );
	virtual ~Table( );

public:
	void update( );
	void memoryFree( );

public:
	char* getData( );
	int getCol( ) const;
	int getRow( ) const;

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
	//����̔z��ԍ�
	int _x;
	int _y;

	//�摜�ɑ΂��Ă̏c��
	int _col;
	int _row;

	//���̑�
	long long int _idx;
	int _handle;
	int _size;

	//�R���C�_�[�f�[�^
	char *_data;


	COMMAND _command;

	MousePtr _mouse;
	DrawerPtr _drawer;
	KeyboardPtr _keyboard;
};

