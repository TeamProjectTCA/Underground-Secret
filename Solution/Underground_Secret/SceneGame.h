#pragma once
#include "Scene.h"
#include "smart_ptr.h"

PTR( SceneGame );
PTR( Keyboard );
PTR( Drawer );
PTR( Map );
PTR( CharacterManager );

class SceneGame : public Scene {
public:
	SceneGame( int stage );
	virtual ~SceneGame( );
	
public:
	void update( );

private:
	int _stage;

	KeyboardPtr _keyboard;
	DrawerPtr _drawer;
	MapPtr _map;
	CharacterManagerPtr _char_manager;
};

