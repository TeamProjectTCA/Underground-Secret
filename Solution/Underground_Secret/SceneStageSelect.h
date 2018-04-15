#pragma once
#include "Scene.h"
#include "smart_ptr.h"

PTR( SceneStageSelect );
PTR( Keyboard );
PTR( Drawer );

class SceneStageSelect : public Scene {
public:
	SceneStageSelect( );
	virtual ~SceneStageSelect( );

public:
	void update( );

private:
	KeyboardPtr _keyboard;
	DrawerPtr _drawer;
};