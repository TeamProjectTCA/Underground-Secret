#pragma once
#include "Scene.h"
#include "smart_ptr.h"

PTR( SceneStageSelect );
PTR( Keyboard );
PTR( Drawer );
PTR( Mouse );
PTR( Button );

class SceneStageSelect : public Scene {
public:
	SceneStageSelect( );
	virtual ~SceneStageSelect( );

public:
	void update( );
	void draw( ) const;
	int getStage( ) const;

private:
	int _stage;

	KeyboardPtr _keyboard;
	DrawerPtr _drawer;
	MousePtr _mouse;
	ButtonPtr _button;
};