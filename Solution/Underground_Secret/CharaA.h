#pragma once
#include "Character.h"
#include "smart_ptr.h"
#include "const.h"

PTR( CharaA );
PTR( Drawer );

class CharaA {
public:
	CharaA( );
	~CharaA( );
public:
	void update( );

private:
	void fall( );
	void colliderSet( );
	void drawCharacter( );

private:
	const int FALL_SPEED = 5;

	int _stage;
	int _chara_handle;
	int _chara_x;
	int _chara_y;
	int _vec_x;
	int _vec_y;
	int _check_x;
	int _check_y;
	DIR _dir;

	DrawerPtr _drawer;
};