#pragma once
#include "smart_ptr.h"
#include "Vector.h"

PTR( Scroll );
PTR( Keyboard );

class Scroll {
public:
	Scroll( );
	virtual ~Scroll( );

public:
	void update( );

public:
	void setCol( const int col );
	void setRow( const int row );

public:
	void setScroll( Vector vec );

public:
	Vector getScroll( ) const;

private:
	Vector _scroll;
	int _col;
	int _row;

	KeyboardPtr _keyboard;
};

