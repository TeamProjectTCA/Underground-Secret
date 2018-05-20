#pragma once
#include "smart_ptr.h"
#include "Vector.h"
#include <vector>

PTR( Shutter );
PTR( Drawer );
PTR( Mouse );

class Shutter {
private:
	enum SHUTTER_STATE {
		SHUTTER_STATE_NON_ACTIVE, // 開いている状態
		SHUTTER_STATE_ACTIVE,     // 閉じている状態
		SHUTTER_STATE_OPEN,       // 開き途中
		SHUTTER_STATE_CLOSE,      // 閉じ途中
	};

public:
	Shutter( );
	virtual ~Shutter( );

public:
	void update( );
	void draw( int col, Vector scroll ) const;

private:
	void action( );

public:
	void addShutter( std::vector< int > shutter );

private:
	int _shutter_width;
	int _shutter_height;
	int _shutter_handle;
	int _move_cnt;
	std::vector< std::vector< int > > _shutter;
	std::vector< SHUTTER_STATE > _shutter_state;

	DrawerPtr _drawer;
	MousePtr _mouse;
};

