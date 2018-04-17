#pragma once
#include "smart_ptr.h"
#include <string>

PTR( Keyboard );
PTR( Drawer );

class FileAction {
public:
	FileAction( );
	virtual ~FileAction( );

public:
	void update( );

protected:
	void writeName( );
	virtual void actionEnter( ) = 0;

protected:
	std::string _input;
	bool _enter;

	KeyboardPtr _keyboard;
	DrawerPtr _drawer;
};

