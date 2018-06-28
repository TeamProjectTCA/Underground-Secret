#pragma once
#include "smart_ptr.h"
#include "Vector.h"

PTR( SpecialElevator );
PTR( Drawer );
PTR( Scroll );

class SpecialElevator {
public:
	SpecialElevator( ScrollConstPtr scroll );
	virtual ~SpecialElevator( );

public:
	void update( );
	void draw( ) const;

private:
	void calcAlpha( );
	void drawAlpha( ) const;
	void calcBeta( );
	void drawBeta( ) const;

public:
	void setElevator( Vector start, Vector end );
	void setImage( const char *path );
	void setActiveAlpha( bool active );
	void setActiveBeta( bool active );

public:
	bool isCloseAlpha( ) const;
	bool isCloseBeta( ) const;

private:
	struct Data {
		bool active;
		Vector pos;
		int cnt;
	};
	Data _alpha;
	Data _beta;
	int _handle;
	int _back_handle;
	int _width;
	int _height;

	DrawerPtr _drawer;
	ScrollConstPtr _scroll;
};

