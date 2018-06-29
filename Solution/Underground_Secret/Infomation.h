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
	int getItemNum( std::string tag ) const;
	void inputInfoController( std::vector< std::string > &info, std::vector< std::string > item, int get_item_num );
	std::string getItem( std::vector< std::string > &item );
	std::vector< std::string > createInfo( CHARACTER chara );

public:
	std::vector< std::string > getInfo( CHARACTER chara );

private:
	struct Values {
		std::string tag;
		std::vector< std::string > value;
	};

	struct Info {
		CHARACTER chara;
		std::vector< Values > value;
	};

	// ƒLƒƒƒ‰ > tag > value
	std::vector< Info > _info;

	LoadCSVPtr _csv;
	RandomPtr _rand;
};

