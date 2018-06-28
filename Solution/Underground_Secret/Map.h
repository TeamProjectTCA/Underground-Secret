#pragma once
#include "smart_ptr.h"
#include "Vector.h"
#include "Phase.h"
#include "Elevator.h"
#include <string>
#include <unordered_map>
#include <vector>

PTR( Map );
PTR( Debug );
PTR( Drawer );
PTR( Keyboard );
PTR( Shutter );
PTR( Scroll );
PTR( Elevator );

class Map {
public:
	Map( int stage, ScrollConstPtr scroll );
	virtual ~Map( );

public:
	void update( );
	void draw( ) const;

public:
	Vector getFixedpointAlpha( PHASE phase ) const;
	Vector getFixedpointBeta( PHASE phase ) const;
	Vector getElevatorPos( char id ) const;
	int getElevatorIdx( char id ) const;
	Vector getScrollData( ) const;
	Vector getMapPos( ) const;
	int getCol( ) const;
	int getRow( ) const;
	int getMapData( int idx ) const;
	char getElevatorId( int idx ) const;
	ELEVATOR_STATE getElevatorState( char id, int idx ) const;
	ELEVATOR_POS getActiveElevator( char id, int idx ) const;
	ELEVATOR_POS getDestination( char id, int idx ) const;
	bool isHitShutter( int detection_idx ) const;

private:
	void loadMap( );
	void setFixedpoint( );
	void setShutter( );
	void setElevator( );
	void inputShutter( std::vector< int > &shutter, int idx );

private:
	void drawCollider( ) const;
	void drawTable( ) const;
	
private:
	int _map_handle;
	int _stage;
	int _row;
	int _col;
	bool _debug_mode;
	std::string _data;
	Vector _fixedpoint_alpha_start;
	Vector _fixedpoint_alpha_play;
	Vector _fixedpoint_alpha_end;
	Vector _fixedpoint_beta_start;
	Vector _fixedpoint_beta_play;
	Vector _fixedpoint_beta_end;
	std::unordered_map< char, ElevatorPtr > _elevator;

	DebugPtr _debug;
	DrawerPtr _drawer;
	KeyboardPtr _keyboard;
	ShutterPtr _shutter;
	ScrollConstPtr _scroll;
};

