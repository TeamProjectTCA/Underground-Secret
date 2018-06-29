#pragma once
#include "smart_ptr.h"
#include "const.h"
#include <vector>
#include <string>
#include <unordered_map>

PTR( Infomation );
PTR( LoadCSV );
PTR( Random );

class Infomation {
public:
	Infomation( );
	virtual ~Infomation( );

private:
	int getItemNum( std::string tag );
	void inputInfoController( std::vector< std::string > &info, std::vector< std::string > item, int get_item_num );
	std::string getItem( std::vector< std::string > &item );

public:
	std::vector< std::string > &getInfo( CHARACTER chara );

private:
	std::unordered_map< CHARACTER, std::vector< std::string > > _info;

	LoadCSVPtr _csv;
	RandomPtr _rand;
};

