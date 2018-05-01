#include "SceneGame.h"
#include "Keyboard.h"
#include "Drawer.h"
#include "Map.h"
#include "CharacterManager.h"
#include "PhaseStart.h"

SceneGame::SceneGame( int stage ) :
_stage( stage ) {
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_map = MapPtr( new Map( _stage ) );
	_char_manager = CharacterManagerPtr( new CharacterManager( ) );
	_run_phase = PhaseStartPtr( new PhaseStart( ) );
}

SceneGame::~SceneGame( ) {
}

void SceneGame::update( ) {
	_map->update( );
	_run_phase->update( );

	switch ( _phase ) {
	case PHASE_START:
		updatePhaseStart( );
		break;

	case PHASE_PLAY:
		updatePhasePlay( );
		break;

	case PHASE_END:
		updatePhaseEnd( );
		break;

	default:
		break;
	}

	PHASE next = _run_phase->getPhase( );
	if ( _phase != next ) {
		_phase = next;
	}

	_drawer->flip( );
}

void SceneGame::updatePhaseStart( ) {
}

void SceneGame::updatePhasePlay( ) {
	_char_manager->update( );
}

void SceneGame::updatePhaseEnd( ) {

}