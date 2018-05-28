#pragma once
#include "smart_ptr.h"
#include "const.h"
#include <list>

PTR( CharacterManager );
PTR( Character );
PTR( Debug );
PTR( Map );
PTR( Infomation );
PTR( CharaA );
PTR( CharaDummy );

class CharacterManager {
public:
	CharacterManager( MapPtr map );
	virtual ~CharacterManager( );

public:
	void update( );

public:
	void addChara( CharacterPtr ptr );
	void setChara( int num );

public:
	std::list< CharacterPtr > getChara( );

private:
	CharacterPtr getCharaInstance( CHARACTER chara_idx );
	
private:
	std::list< CharacterPtr > _chara;
	bool _debug_mode;

	MapPtr _map;
	DebugPtr _debug;
	InfomationPtr _info;
	CharaAPtr _chara_a;
	CharaDummyPtr _chara_dummy1;
	CharaDummyPtr _chara_dummy2;
	CharaDummyPtr _chara_dummy3;
};

