#pragma once
#include "Character.h"
#include "smart_ptr.h"
#include "const.h"

PTR( CharaA );
PTR( Map );
PTR( Drawer );
PTR( Keyboard );
PTR( Infomation );

class CharaA : public Character {
public:
	CharaA( MapPtr map, InfomationPtr info );
	virtual ~CharaA( );
public:
	void update( );

private:
	void fall( );
	void move( );

public:
	void setFall( bool fall );
	void setMove( bool move );
	void setMoveUp( bool moveup );

	int getWidth( ) const;
	int getHeight( ) const;
	DIR getDir( ) const;

private:
	const int FALL_SPEED = 2;

	bool _enable_fall;
	bool _enable_move;
	bool _enable_move_up;
	int _stage;
	int _vec_x;
	int _vec_y;
	DIR _dir;

	DrawerPtr _drawer;
	KeyboardPtr _keyboard;
};