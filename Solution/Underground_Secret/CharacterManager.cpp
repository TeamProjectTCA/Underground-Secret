#include "CharacterManager.h"
#include "Map.h"
#include "CharaA.h"
#include "CharaDummy.h"

CharacterManager::CharacterManager( MapPtr map ) :
_map( map ) {
	_chara_a = CharaAPtr( new CharaA( _map ) );

	// debug
	_chara_dummy = CharaDummyPtr( new CharaDummy( _map ) );
	addChara( _chara_dummy );
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
}

void CharacterManager::addChara( CharacterPtr ptr ) {
	_chara.push_back( ptr );
}

std::list< CharacterPtr > CharacterManager::getChara( ) {
	return _chara;
}