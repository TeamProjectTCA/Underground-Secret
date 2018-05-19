#include "Infomation.h"
#include "LoadCSV.h"
#include "Random.h"

Infomation::Infomation( ) {
	_rand = RandomPtr( new Random( ) );

	// csvから情報をすべて取得
	_csv = LoadCSVPtr( new LoadCSV( "Resources/CharacterData/" ) );
	std::unordered_map< CHARACTER, std::vector< CsvData > > data;
	//_csv->read( data[ CHARA_A ]    , "Chara_A" );
	_csv->read( data[ CHARA_DUMMY ], "Chara_Dummy" );

	// csvから取り出した情報をキャラごとにひとまとめにしていく
	std::unordered_map< CHARACTER, std::vector< std::string > > all_info;
	for ( int i = 0; i < CHARA_MAX; i++ ) {
		CHARACTER chara = ( CHARACTER )i;
		if ( data.find( chara ) == data.end( ) ) {
			continue;
		}

		int size = ( int )data[ chara ].size( );
		for ( int j = 0; j < size; j++ ) {
			convCsvdataToInfodata( all_info[ chara ], chara, data[ chara ][ j ].values );
		}
	}

	// 全ての情報から特定数だけ取得する
	std::unordered_map< CHARACTER, std::vector< std::string > >::iterator ite;
	ite = all_info.begin( );
	for ( ite; ite != all_info.end( ); ite++ ) {
		setUseInfo( ite->first, ite->second );
	}
}

Infomation::~Infomation( ) {
}

void Infomation::convCsvdataToInfodata( std::vector< std::string > &input, CHARACTER chara, std::vector< std::string > &data ) {
	int size = ( int )data.size( );
	for ( int i = 0; i < size; i++ ) {
		input.push_back( data[ i ] );
	}
}

void Infomation::setUseInfo( CHARACTER chara, std::vector< std::string > &data ) {
	int size = ( int )data.size( );
	_rand->setRange( 0, size - 1 );
	for ( int i = 0; i < size; i++ ) {
		// 乱数を取得
		int num = _rand->getInt32( );

		// 入力
		_info[ chara ].push_back( data[ num ] );

		// 同じものが選択されないようにする
		_rand->setIgnore( ( unsigned long )num );

		if ( INFO_MAX == i + 1 ) {
			break;
		}
	}
	_rand->resetRange( );
	_rand->resetIgnore( );
}

std::vector< std::string > &Infomation::getInfo( CHARACTER chara ) {
	return _info[ chara ];
}
