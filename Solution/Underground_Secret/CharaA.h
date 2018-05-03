#pragma once
#include "Character.h"
#include "smart_ptr.h"

PTR( CharaA );
PTR( Drawer );

class CharaA : public Character {
public:
	CharaA( );
	~CharaA( );
public:
	void update( );

private:
	void drawCharacter( );

private:
	int _chara_handle;
	int _chara_x;
	int _chara_y;

	DrawerPtr _drawer;
};