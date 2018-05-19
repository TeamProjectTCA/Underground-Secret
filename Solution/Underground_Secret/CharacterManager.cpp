#include "CharacterManager.h"
#include "Debug.h"
#include "Map.h"
#include "Infomation.h"
#include "CharaA.h"
#include "CharaDummy.h"

CharacterManager::CharacterManager( MapPtr map ) :
_map( map ) {
	_debug_mode = false;

	_debug = Debug::getTask( );
	_info = InfomationPtr( new Infomation( ) );

	_chara_a = CharaAPtr( new CharaA( _map, _info ) );

	// debug
	_chara_dummy1 = CharaDummyPtr( new CharaDummy( _map, _info ) );
	_chara_dummy2 = CharaDummyPtr( new CharaDummy( _map, _info ) );
	_chara_dummy3 = CharaDummyPtr( new CharaDummy( _map, _info ) );
	addChara( _chara_dummy1 );
	//addChara( _chara_dummy2 );
	//addChara( _chara_dummy3 );
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

std::list< CharacterPtr > CharacterManager::getChara( ) {
	return _chara;
}