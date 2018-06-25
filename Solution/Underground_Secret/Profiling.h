#pragma once
#include "smart_ptr.h"
#include "const.h"
#include <vector>
#include <string>
#include <unordered_map>

PTR( Profiling );
PTR( Mouse );
PTR( Sound );
PTR( Drawer );
PTR( Button );

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
	std::unordered_map< SOUND, int > _sound_handle;
	std::vector< std::string > _profiling;

	MousePtr _mouse;
	SoundPtr _sound;
	DrawerPtr _drawer;
	ButtonPtr _button;
};

