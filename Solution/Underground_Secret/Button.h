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
	void draw( bool transflag = true ) const;

public:
	void setCollider( float lx, float ly, float rx, float ry );
	void setPos( float x1, float y1, float x2 = -1.0f, float y2 = -1.0f );
	void setImage( const char* filepath );
	void setPushImage( const char* filepath );

public:
	Vector getColliderLeft( ) const;
	Vector getColliderRight( ) const;
	Vector getPosCenter( ) const;
	
public:
	inline void click( Vector mouse ) {
		_state = BUTTON_STATE_NORMAL;

		if ( _lx <= mouse.x && mouse.x <= _rx &&
			 _ly <= mouse.y && mouse.y <= _ry ) {
			_state = BUTTON_STATE_PUSH;
		}
	}
	inline bool isPush( ) const {
		return ( _state == BUTTON_STATE_PUSH ? true : false );
	}
	inline void resetState( ) {
		_state = BUTTON_STATE_NORMAL;
	}

private:
	int _handle;
	int _push_handle;

	// •`‰æ—p‚Ì•Ï”
	float _x1;
	float _y1;
	float _x2;
	float _y2;

	// “–‚½‚è”»’è—p‚Ì•Ï”
	float _lx;
	float _ly;
	float _rx;
	float _ry;
	BUTTON_STATE _state;
	DrawerPtr _drawer;
};

