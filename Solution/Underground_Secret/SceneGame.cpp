#include "SceneGame.h"
#include "Keyboard.h"
#include "Drawer.h"
#include "Map.h"
#include "CharacterManager.h"
#include "PhaseStart.h"
#include "PhasePlay.h"
#include "PhaseEnd.h"
#include "Scroll.h"

const int ONE_STAGE_CHARACTER_NUM = 1;

SceneGame::SceneGame( int stage ) :
_stage( stage ) {
	_drawer = Drawer::getTask( );
	_keyboard = Keyboard::getTask( );
	_scroll = ScrollPtr( new Scroll( ) );
	_map = MapPtr( new Map( _stage, _scroll ) );
	_char_manager = CharacterManagerPtr( new CharacterManager( _map ) );
	_char_manager->setChara( ONE_STAGE_CHARACTER_NUM );

	// スクロールに col,row を詰める
	_scroll->setCol( _map->getCol( ) );
	_scroll->setRow( _map->getRow( ) );

	_phase = PHASE_START;
	changePhase( );
}

SceneGame::~SceneGame( ) {
}

void SceneGame::update( ) {
	_map->update( );
	_run_phase->update( );
	// phase によって変わる処理( 外部クラス )
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
	_drawer->flip( );

	PHASE next = _run_phase->getPhase( );
	if ( next == PHASE_CONTINUE ) {
		return;
	}

	if ( _phase != next ) {
		_phase = next;
		changePhase( );
		_map->setPhase( _phase );
	}

}

void SceneGame::updatePhaseStart( ) {
}

void SceneGame::updatePhasePlay( ) {
	_char_manager->update( );
}

void SceneGame::updatePhaseEnd( ) {

}

void SceneGame::changePhase( ) {
	switch ( _phase ) {
	case PHASE_START:
		_run_phase = PhaseStartPtr( new PhaseStart( _char_manager->getChara( ), _map ) );
		break;

	case PHASE_PLAY:
		_run_phase = PhasePlayPtr( new PhasePlay( _char_manager->getChara( ), _scroll ) );
		break;

	case PHASE_END:
		_run_phase = PhaseEndPtr( new PhaseEnd( ) );
		break;

	default:
		break;
	}
}

