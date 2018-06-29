#include "CharacterManager.h"
#include "Map.h"
#include "Infomation.h"
#include "CharaDummy.h"
#include "Man1.h"
#include "Man2.h"
#include "Man3.h"
#include "Woman1.h"
#include "Woman2.h"
#include "Woman3.h"
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

	case CHARA_DUMMY : return CharaDummyPtr( new CharaDummy( _map, _info ) );
	case CHARA_MAN1  : return Man1Ptr      ( new Man1      ( _map, _info ) );
	case CHARA_MAN2  : return Man2Ptr      ( new Man2      ( _map, _info ) );
	case CHARA_MAN3  : return Man3Ptr      ( new Man3      ( _map, _info ) );
	case CHARA_WOMAN1: return Woman1Ptr    ( new Woman1    ( _map, _info ) );
	case CHARA_WOMAN2: return Woman2Ptr    ( new Woman2    ( _map, _info ) );
	case CHARA_WOMAN3: return Woman3Ptr    ( new Woman3    ( _map, _info ) );
	}
	return CharacterPtr( );
}