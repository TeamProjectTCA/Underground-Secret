#pragma once
#include "smart_ptr.h"
#include <string>

PTR( Keyboard );
PTR( Drawer );
PTR( Mouse );

class FileAction {
public:
	FileAction( );
	virtual ~FileAction( );

public:
	void update( );
	bool isFin( ) const;

protected:
	void writeName( );
	virtual void actionEnter( ) = 0;

protected:
	bool _fin;
	std::string _input;

	KeyboardPtr _keyboard;
	MousePtr _mouse;
	DrawerPtr _drawer;
};

