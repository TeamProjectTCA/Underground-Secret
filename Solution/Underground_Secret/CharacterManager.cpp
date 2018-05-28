#include "CharacterManager.h"
#include "Debug.h"
#include "Map.h"
#include "Infomation.h"
#include "CharaA.h"
#include "CharaDummy.h"
#include "Random.h"

CharacterManager::CharacterManager( MapPtr map ) :
_map( map ) {
	_debug_mode = false;

	_debug = Debug::getTask( );
	_info = InfomationPtr( new Infomation( ) );
}

CharacterManager::~CharacterManager( ) {
}

void CharacterManager::update( ) {
	std::list< CharacterPtr >::iterator ite;
	ite = _chara.begin( );
	for ( ite; ite != _chara.end( ); ite++ ) {
		( *ite )->update( );
		( *ite )->draw( );
	}

	bool debug = _debug->isDebug( );
	if ( _debug_mode != debug ) {
		_debug_mode = debug;

		ite = _chara.begin( );
		for ( ite; ite != _chara.end( ); ite++ ) {
			( *ite )->changeDebugMode( );
		}
	}
}

void CharacterManager::addChara( CharacterPtr ptr ) {
	_chara.push_back( ptr );
}

void CharacterManager::setChara( int num ) {
	RandomPtr random = RandomPtr( new Random( ) );
	random->setRange( 0, CHARA_MAX - 1 );

	for ( int i = 0; i < num; i++ ) {
		// óêêîÇ≈ê∂ê¨ÉLÉÉÉâÇÃî‘çÜÇéÊìæ
		int chara_idx = random->getInt32( );

		CharacterPtr chara = getCharaInstance( ( CHARACTER )chara_idx );
		// errorèàóù
		if ( chara == CharacterPtr( ) ) {
			continue;
		}

		// addÇ∑ÇÈ
		addChara( chara );
	}
}

std::list< CharacterPtr > CharacterManager::getChara( ) {
	return _chara;
}

CharacterPtr CharacterManager::getCharaInstance( CHARACTER chara_idx ) {
	switch ( chara_idx ) {
	case CHARA_A    : return CharaAPtr    ( new CharaA    ( _map, _info ) );
	case CHARA_DUMMY: return CharaDummyPtr( new CharaDummy( _map, _info ) );
	}
	return CharacterPtr( );
}