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
	void setPhase( PHASE phase );

public:
	Vector getFixedpointAlpha( PHASE phase ) const;
	Vector getFixedpointBeta( PHASE phase ) const;
	Vector getScrollData( ) const;

private:
	void loadMap( );
	void setFixedpoint( );
	void scroll( );

private:
	void drawCollider( ) const;
	void drawTable( ) const;

private:
	int _handle;
	int _stage;
	int _row;
	int _col;
	PHASE _phase;
	Vector _scroll;
	bool _debug;
	std::string _data;
	Vector _fixedpoint_alpha_start;
	Vector _fixedpoint_alpha_play;
	Vector _fixedpoint_alpha_end;
	Vector _fixedpoint_beta_start;
	Vector _fixedpoint_beta_play;
	Vector _fixedpoint_beta_end;

	DrawerPtr _drawer;
	KeyboardPtr _keyboard;
};

