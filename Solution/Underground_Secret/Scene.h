#pragma once
#include "smart_ptr.h"

PTR ( Scene );

enum SCENE {
	SCENE_CONTINUE,//åªèÛà€éù
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_STAGESELECT,
	SCENE_SCENE_MAX,
};

class Scene {
public:
	Scene( );
	virtual ~Scene( );

public:
	virtual void update( ) = 0;
	void setNextScene( SCENE next );
	SCENE getNextScene( ) const;

private:
	SCENE _next_scene;
};

