#pragma once
#include "smart_ptr.h"

PTR( Profiling );
PTR( Mouse );
PTR( Drawer );

class Profiling {
public:
	Profiling( );
	virtual ~Profiling( );

public:
	void update( );
	void draw( );

private:
	void calcActiveButton( );
	void calcBoardPos( );

private:
	void drawActiveButton( ) const;
	void drawBoard( ) const;

public:
	bool isActive( ) const;

private:
	bool _active;
	int _active_button_handle;
	int _active_button_width;
	int _active_button_height;
	int _back_button;
	int _board_handle;
	float _board_y;
	int _board_count;

	MousePtr _mouse;
	DrawerPtr _drawer;
};

