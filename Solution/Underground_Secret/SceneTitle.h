#pragma once
#include "Scene.h"
#include "smart_ptr.h"
#include <string>

PTR( SceneTitle );
PTR( Drawer );
PTR( Keyboard );

class SceneTitle : public Scene {
public:
	SceneTitle( );
	virtual ~SceneTitle( );

public:
	void update( );

private:
	//�ϐ�
	int _background_handle;

	//�N���X
	DrawerPtr _drawer;
	KeyboardPtr _keyboard;
};

