#pragma once
#include "smart_ptr.h"
#include "Vector.h"
#include "Phase.h"
#include <string>
#include <map>
#include <vector>

PTR( Map );
PTR( Debug );
PTR( Drawer );
PTR( Keyboard );
PTR( Shutter );

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
	Vector getElevatorPos( int ascii ) const;
	int getCol( ) const;
	int getMapData( int idx ) const;
	bool isHitShutter( int detection_idx ) const;

private:
	void draw( );
	void loadMap( );
	void setFixedpoint( );
	void setShutter( );
	void inputShutter( std::vector< int > &shutter, int idx );
	void scroll( );

private:
	void drawCollider( ) const;
	void drawTable( ) const;

private:
	int _map_handle;
	int _stage;
	int _row;
	int _col;
	PHASE _phase;
	Vector _scroll;
	bool _debug_mode;
	std::string _data;
	Vector _fixedpoint_alpha_start;
	Vector _fixedpoint_alpha_play;
	Vector _fixedpoint_alpha_end;
	Vector _fixedpoint_beta_start;
	Vector _fixedpoint_beta_play;
	Vector _fixedpoint_beta_end;

	DebugPtr _debug;
	DrawerPtr _drawer;
	KeyboardPtr _keyboard;
	ShutterPtr _shutter;
};

