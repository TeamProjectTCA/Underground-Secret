#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include <string>
#include <list>

PTR( Debug );
PTR( Keyboard );
PTR( Drawer );

class Debug : public Base {
public:
	static DebugPtr getTask( );
	static std::string getTag( );

public:
	Debug( );
	virtual ~Debug( );

public:
	void initialize( );
	void update( );
	void draw( ) const;

public:
	void drawLog( ) const;

public:
	void addLog( std::string log );
	void clearLog( );

public:
	bool isDebug( ) const;

private:
	bool _debug;
	std::list< std::string > _log;

	KeyboardPtr _keyboard;
	DrawerPtr _drawer;
};

