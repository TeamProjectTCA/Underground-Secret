#include "CharacterManager.h"
#include "Map.h"
#include "Infomation.h"
#include "CharaA.h"
#include "CharaDummy.h"
#include "Random.h"

CharacterManager::CharacterManager( MapPtr map ) :
_map( map ),
_spy_idx( -1 ) {

	_info = InfomationPtr( new Infomation( ) );
}

CharacterManager::~CharacterManager( ) {
}

void CharacterManager::update( ) {
	std::list< CharacterPtr >::iterator ite;
	ite = _chara.begin( );
	for ( ite; ite != _chara.end( ); ite++ ) {
		( *ite )->update( );
	}
}

void CharacterManager::draw( ) const {
	std::list< CharacterPtr >::const_iterator ite;
	ite = _chara.begin( );
	for ( ite; ite != _chara.end( ); ite++ ) {
		( *ite )->draw( );
	}
}

void CharacterManager::addChara( CharacterPtr ptr ) {
	_chara.push_back( ptr );
}

void CharacterManager::setChara( int num ) {
	RandomPtr random = Random::getTask( );

	for ( int i = 0; i < num; i++ ) {
		// �����Ő����L�����̔ԍ����擾
		int chara_idx = random->getInt32( 0, CHARA_MAX - 1 );

		CharacterPtr chara = getCharaInstance( ( CHARACTER )chara_idx );
		// error����
		if ( chara == CharacterPtr( ) ) {
			continue;
		}

		// add����
		addChara( chara );
	}

	// �X�p�C�����߂�
	_spy_idx = random->getInt32( 0, ( int )_chara.size( ) - 1 );
}

std::list< CharacterPtr > CharacterManager::getChara( ) {
	return _chara;
}

int CharacterManager::getSpyIdx( ) const {
	return _spy_idx;
}

CharacterPtr CharacterManager::getCharaInstance( CHARACTER chara_idx ) {
	switch ( chara_idx ) {
	case CHARA_A    : return CharaAPtr    ( new CharaA    ( _map, _info ) );
	case CHARA_DUMMY: return CharaDummyPtr( new CharaDummy( _map, _info ) );
	}
	return CharacterPtr( );
}