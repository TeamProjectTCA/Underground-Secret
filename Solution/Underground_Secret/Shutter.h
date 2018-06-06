#pragma once
#include "smart_ptr.h"
#include "Vector.h"
#include "map.h"
#include <vector>

PTR( Shutter );
PTR( Drawer );
PTR( Mouse );

class Shutter {
private:
	enum SHUTTER_STATE {
		SHUTTER_STATE_NONACTIVE,  // ŠJ‚¢‚Ä‚¢‚éó‘Ô
		SHUTTER_STATE_ACTIVE,     // •Â‚¶‚Ä‚¢‚éó‘Ô
		SHUTTER_STATE_OPEN,       // ŠJ‚«“r’†
		SHUTTER_STATE_CLOSE,      // •Â‚¶“r’†
	};

public:
	Shutter( int stage );
	virtual ~Shutter( );

public:
	void update( );
	void draw( ) const;

private:
	void onShutter( );
	void calcShutter( );

public:
	void setCol( const int col );
	void setScroll( Vector scroll );
	void setEndScroll( Vector end_scroll );
	void addShutter( std::vector< int > shutter );

public:
	bool isHitShutter( int detection_idx ) const;

private:
	const int ON_SHUTTER_MAX;
	int _col;
	int _shutter_width;
	int _shutter_height;
	int _shutter_handle;
	int _move_cnt;
	int _active_num;
	Vector _scroll;
	Vector _end_scroll;
	std::vector< std::vector< int > > _shutter;
	std::vector< SHUTTER_STATE > _shutter_state;

	DrawerPtr _drawer;
	MousePtr _mouse;
};

