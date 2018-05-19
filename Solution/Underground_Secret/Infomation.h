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
	void convCsvdataToInfodata( std::vector< std::string > &input, CHARACTER chara, std::vector< std::string > &data );
	void setUseInfo( CHARACTER chara, std::vector< std::string > &data );

public:
	std::vector< std::string > &getInfo( CHARACTER chara );

private:
	std::unordered_map< CHARACTER, std::vector< std::string > > _info;

	LoadCSVPtr _csv;
	RandomPtr _rand;
};

