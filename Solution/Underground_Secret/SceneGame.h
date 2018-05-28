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
PTR( Scroll );
PTR( Debug );

class SceneGame : public Scene {
public:
	SceneGame( int stage );
	virtual ~SceneGame( );
	
public:
	void update( );
	void draw( );

private:
	void updatePhaseStart( );
	void updatePhasePlay( );
	void updatePhaseEnd( );
	void changePhase( );

private:
	PHASE _phase;
	int _stage;

	KeyboardPtr _keyboard;
	DrawerPtr _drawer;
	MapPtr _map;
	CharacterManagerPtr _char_manager;
	PhasePtr _run_phase;
	ScrollPtr _scroll;
	DebugConstPtr _debug;
};

