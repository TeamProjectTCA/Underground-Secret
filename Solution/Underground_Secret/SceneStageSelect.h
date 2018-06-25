#pragma once
#include "Scene.h"
#include "smart_ptr.h"
#include <vector>

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
	void createButton( );

private:
	int _stage;
	int _back_image;

	KeyboardPtr _keyboard;
	DrawerPtr _drawer;
	MousePtr _mouse;
	std::vector< ButtonPtr > _button;
};