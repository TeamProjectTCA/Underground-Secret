#pragma once
#include "smart_ptr.h"
#include "const.h"
#include <list>

PTR( CharacterManager );
PTR( Character );
PTR( Map );
PTR( Infomation );
PTR( CharaDummy );
PTR( CharaMan1 );
PTR( CharaWoman2 );

class CharacterManager {
public:
	CharacterManager( MapPtr map );
	virtual ~CharacterManager( );

public:
	void update( );
	void draw( ) const;

public:
	void addChara( CharacterPtr ptr );
	void setChara( int num );

public:
	std::list< CharacterPtr > getChara( );

private:
	CharacterPtr getCharaInstance( CHARACTER chara_idx );
	
private:
	std::list< CharacterPtr > _chara;

	MapPtr _map;
	InfomationPtr _info;
};

