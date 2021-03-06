#pragma once
#include "smart_ptr.h"
#include "Vector.h"
#include "map.h"
#include <vector>

PTR( Shutter );
PTR( Drawer );
PTR( Mouse );
PTR( Button );

class Shutter {
private:
	enum SHUTTER_STATE {
		SHUTTER_STATE_NONACTIVE,  // 開いている状態
		SHUTTER_STATE_ACTIVE,     // 閉じている状態
		SHUTTER_STATE_OPEN,       // 開き途中
		SHUTTER_STATE_CLOSE,      // 閉じ途中
	};
	
public:
	Shutter( int stage );
	virtual ~Shutter( );

public:
	void update( );
	void draw( );

private:
	void onShutter( );
	void calcShutter( );

public:
	void setCol( const int col );
	void setScroll( Vector scroll );
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
	std::vector< std::vector< int > > _shutter;
	std::vector< SHUTTER_STATE > _shutter_state;

	DrawerPtr _drawer;
	MousePtr _mouse;
	std::vector< ButtonPtr > _button;
};

