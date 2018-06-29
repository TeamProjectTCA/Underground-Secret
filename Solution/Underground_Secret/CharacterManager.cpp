#include "CharacterManager.h"
#include "Map.h"
#include "Infomation.h"
#include "CharaMan1.h"
#include "CharaMan2.h"
#include "CharaMan3.h"
#include "CharaWoman1.h"
#include "CharaWoman2.h"
#include "CharaWoman3.h"
#include "Random.h"

CharacterManager::CharacterManager( MapPtr map ) :
_map( map ) {
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
		// 乱数で生成キャラの番号を取得
		int chara_idx = random->getInt32( 0, CHARA_MAX - 1 );

		CharacterPtr chara = getCharaInstance( ( CHARACTER )chara_idx );
		// error処理
		if ( chara == CharacterPtr( ) ) {
			continue;
		}

		// addする
		addChara( chara );
	}

	// スパイを決める
	int spy_idx = random->getInt32( 0, ( int )_chara.size( ) - 1 );
	std::list< CharacterPtr >::iterator ite;
	ite = _chara.begin( );
	for ( int i = 0; i < spy_idx; i++ ) {
		ite++;
	}
	( *ite )->setSpy( true );
}

std::list< CharacterPtr > CharacterManager::getChara( ) {
	return _chara;
}

CharacterPtr CharacterManager::getCharaInstance( CHARACTER chara_idx ) {
	switch ( chara_idx ) {
	case CHARA_MAN1  : return CharaMan1Ptr      ( new CharaMan1      ( _map, _info ) );
	case CHARA_MAN2  : return CharaMan2Ptr      ( new CharaMan2      ( _map, _info ) );
	case CHARA_MAN3  : return CharaMan3Ptr      ( new CharaMan3      ( _map, _info ) );
	case CHARA_WOMAN1: return CharaWoman1Ptr    ( new CharaWoman1    ( _map, _info ) );
	case CHARA_WOMAN2: return CharaWoman2Ptr    ( new CharaWoman2    ( _map, _info ) );
	case CHARA_WOMAN3: return CharaWoman3Ptr    ( new CharaWoman3    ( _map, _info ) );
	}
	return CharacterPtr( );
}