#pragma once
#include "smart_ptr.h"
#include <string>

PTR( Map );
PTR( Drawer );
PTR( Keyboard );
PTR( CharaA );

class Map {
public:
	Map( int stage );
	virtual ~Map( );

public:
	void update( );

private:
	void loadMap( );
	void ColliderSet( );

	void drawCollider( ) const;
	void drawTable( ) const;

private:
	int _handle;
	int _stage;
	int _row;
	int _col;
	bool _debug;
	std::string _data;

	DrawerPtr _drawer;
	KeyboardPtr _keyboard;
	CharaAPtr _charaA;
};

