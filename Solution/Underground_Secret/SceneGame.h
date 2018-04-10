#pragma once
#include "Scene.h"
#include "smart_ptr.h"

PTR( SceneGame );
PTR( Keyboard );

class SceneGame : public Scene {
public:
	SceneGame( );
	virtual ~SceneGame( );
	
public:
	void update( );

private:
	KeyboardPtr _keyboard;
};

