#pragma once
#include "Scene.h"
#include "smart_ptr.h"
#include "Phase.h"

PTR( SceneGame );
PTR( Keyboard );
PTR( Drawer );
PTR( Map );
PTR( CharacterManager );
PTR( Phase );

class SceneGame : public Scene {
public:
	SceneGame( int stage );
	virtual ~SceneGame( );
	
public:
	void update( );
	void updatePhaseStart( );
	void updatePhasePlay( );
	void updatePhaseEnd( );

private:
	PHASE _phase;
	int _stage;

	KeyboardPtr _keyboard;
	DrawerPtr _drawer;
	MapPtr _map;
	CharacterManagerPtr _char_manager;
	PhasePtr _run_phase;
};

