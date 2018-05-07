#pragma once
#include "Character.h"
#include "smart_ptr.h"
#include "const.h"

PTR( CharaA );
PTR( Drawer );
PTR( Keyboard );

class CharaA : public Character {
public:
	CharaA( );
	virtual ~CharaA( );
public:
	void update( );

private:
	void fall( );
	void move( );

	void drawCharacter( ) const;

public:
	void setFall( bool fall );
	void setMove( bool move );
	void setMoveUp( bool moveup );

	int getPosX( ) const;
	int getPosY( ) const;
	int getWidth( ) const;
	int getHeight( ) const;
	DIR getDir( ) const;

private:
	const int FALL_SPEED = 2;

	bool _enable_fall;
	bool _enable_move;
	bool _enable_move_up;
	int _stage;
	int _chara_handle;
	int _vec_x;
	int _vec_y;
	DIR _dir;

	DrawerPtr _drawer;
	KeyboardPtr _keyboard;
};