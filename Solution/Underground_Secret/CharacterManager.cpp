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
	RandomPtr random = RandomPtr( new Random( ) );
	random->setRange( 0, CHARA_MAX - 1 );

	for ( int i = 0; i < num; i++ ) {
		// 乱数で生成キャラの番号を取得
		int chara_idx = random->getInt32( );

		CharacterPtr chara = getCharaInstance( ( CHARACTER )chara_idx );
		// error処理
		if ( chara == CharacterPtr( ) ) {
			continue;
		}

		// addする
		addChara( chara );
	}

	// スパイを決める
	random->setRange( 0, num - 1 );
	_spy_idx = random->getInt32( );
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