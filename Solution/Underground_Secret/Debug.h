#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include <string>

PTR( Debug );
PTR( Keyboard );

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

public:
	bool isDebug( ) const;

private:
	bool _debug;

	KeyboardPtr _keyboard;
};

