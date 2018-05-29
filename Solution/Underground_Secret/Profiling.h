#pragma once
#include "smart_ptr.h"
#include <vector>
#include <string>

PTR( Profiling );
PTR( Mouse );
PTR( Drawer );

class Profiling {
public:
	Profiling( std::vector< std::string > &profiling );
	virtual ~Profiling( );

public:
	void update( );
	void draw( ) const;

private:
	void calcActiveButton( );
	void calcBoardPos( );

private:
	void drawActiveButton( ) const;
	void drawBoard( ) const;
	void drawProfiling( ) const;

public:
	bool isActive( ) const;

private:
	bool _active;
	int _active_button_handle;
	int _active_button_width;
	int _active_button_height;
	int _back_button;
	int _board_handle;
	int _board_width;
	int _board_height;
	float _board_y;
	int _board_count;
	std::vector< std::string > _profiling;

	MousePtr _mouse;
	DrawerPtr _drawer;
};

