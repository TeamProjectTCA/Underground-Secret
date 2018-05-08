#pragma once
#include "smart_ptr.h"
#include "Vector.h"
#include "Phase.h"
#include <string>

PTR( Map );
PTR( Drawer );
PTR( Keyboard );

class Map {
public:
	Map( int stage );
	virtual ~Map( );

public:
	void update( );

public:
	Vector getFixedpoint( PHASE phase ) const;

private:
	void loadMap( );
	void setFixedpoint( );

private:
	void drawCollider( ) const;
	void drawTable( ) const;

private:
	int _handle;
	int _stage;
	int _row;
	int _col;
	bool _debug;
	std::string _data;
	Vector _fixed_point_start;
	Vector _fixed_point_play;
	Vector _fixed_point_end;

	DrawerPtr _drawer;
	KeyboardPtr _keyboard;
};

