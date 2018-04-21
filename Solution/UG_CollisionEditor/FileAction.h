#pragma once
#include "smart_ptr.h"
#include <string>

PTR( Keyboard );
PTR( Drawer );
PTR( Mouse );

class FileAction {
public:
	FileAction( std::string action_name );
	virtual ~FileAction( );

public:
	void update( );
	void setGauge( );
	bool isFin( ) const;

protected:
	void writeName( );
	virtual void actionEnter( ) = 0;
	virtual void upGauge( );

protected:
	bool _fin;
	bool _enter;
	bool _gauge_up;
	std::string _input;
	std::string _action_name;

	KeyboardPtr _keyboard;
	MousePtr _mouse;
	DrawerPtr _drawer;
};

