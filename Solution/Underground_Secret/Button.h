#pragma once
#include "smart_ptr.h"
#include "Vector.h"

PTR( Button );
PTR( Drawer );


class Button {
private:
	enum BUTTON_STATE {
		BUTTON_STATE_NORMAL,
		BUTTON_STATE_PUSH,
		BUTTON_STATE_MAX
	};

public:
	Button( float lx = 0, float ly = 0, float rx = 0, float ry = 0 );
	virtual ~Button( );

public:
	void draw( ) const;

public:
	void setPos( float lx, float ly, float rx, float ry );
	void setImage( const char* filepath );
	void setPushImage( const char* filepath );
	
public:
	inline bool click( Vector mouse );

private:
	inline void setState( BUTTON_STATE state );

private:
	int _handle;
	int _push_handle;
	float _lx;
	float _ly;
	float _rx;
	float _ry;
	bool _push;
	BUTTON_STATE _state;
	DrawerPtr _drawer;
};

