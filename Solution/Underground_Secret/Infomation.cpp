#include "Infomation.h"
#include "LoadCSV.h"
#include "Random.h"

const char CHARACTER_DUMMY[ ]  = "Chara_Dummy";
const char CHARACTER_MAN1[ ]   = "man1";
const char CHARACTER_MAN2[ ]   = "man2";
const char CHARACTER_MAN3[ ]   = "man3";
const char CHARACTER_WOMAN1[ ] = "woman1";
const char CHARACTER_WOMAN2[ ] = "woman2";
const char CHARACTER_WOMAN3[ ] = "woman3";

Infomation::Infomation( ) {
	_rand = Random::getTask( );

	// csvÇ©ÇÁèÓïÒÇÇ∑Ç◊ÇƒéÊìæ
	_csv = LoadCSVPtr( new LoadCSV( "Resources/CharacterData/" ) );
	std::unordered_map< CHARACTER, std::vector< CsvData > > data;
	_csv->read( data[ CHARA_DUMMY ] , CHARACTER_DUMMY );
	_csv->read( data[ CHARA_MAN1 ]  , CHARACTER_MAN1 );
	_csv->read( data[ CHARA_MAN2 ]  , CHARACTER_MAN2 );
	_csv->read( data[ CHARA_MAN3 ]  , CHARACTER_MAN3 );
	_csv->read( data[ CHARA_WOMAN1 ], CHARACTER_WOMAN1 );
	_csv->read( data[ CHARA_WOMAN2 ], CHARACTER_WOMAN2 );
	_csv->read( data[ CHARA_WOMAN3 ], CHARACTER_WOMAN3 );

	for ( int i = 0; i < CHARA_MAX; i++ ) {
		CHARACTER chara = ( CHARACTER )i;
		if ( data.find( chara ) == data.end( ) ) {
			continue;
		}
		
		for ( int j = 0; j < data[ chara ].size( ); j++ ) {
			inputInfoController( _info[ chara ], data[ chara ][ j ].values, getItemNum( data[ chara ][ j ].tag ) );
		}
	}
}

Infomation::~Infomation( ) {
}

int Infomation::getItemNum( std::string tag ) {
	int num = 1;

	if ( tag == "CLOTHES" ) {
		num = 1;
	}
	if ( tag == "ACCESSORY" ) {
		num = 2;
	}
	if ( tag == "PERSONALITY" ) {
		num = 1;
	}
	if ( tag == "OTHER" ) {
		num = 1;
	}

	return num;
}

void Infomation::inputInfoController( std::vector< std::string > &info, std::vector< std::string > item, int get_item_num ) {
	for ( int i = 0; i < get_item_num; i++ ) {
		info.push_back( getItem( item ) );
	}
	_rand->resetIgnore( );
}

std::string Infomation::getItem( std::vector< std::string > &item ) {
	int idx = _rand->getInt32( 0, ( int )item.size( ) - 1 );
	if ( idx >= item.size( ) ) {
		return "";
	}

	_rand->setIgnore( ( unsigned long )idx );
	std::string get_item = item[ idx ];
	return get_item;
}

std::vector< std::string > &Infomation::getInfo( CHARACTER chara ) {
	return _info[ chara ];
}
