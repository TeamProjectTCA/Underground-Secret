#pragma once
#include "Scene.h"
#include "smart_ptr.h"

PTR( SceneGame );
PTR( Keyboard );
PTR( Drawer );
PTR( Map );


class SceneGame : public Scene {
public:
	SceneGame( );
	virtual ~SceneGame( );
	
public:
	void update( );

private:
	KeyboardPtr _keyboard;
	DrawerPtr _drawer;
	MapPtr _map;
};

