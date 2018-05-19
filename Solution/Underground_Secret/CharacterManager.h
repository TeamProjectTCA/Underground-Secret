#pragma once
#include "smart_ptr.h"
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
	void addChara( CharacterPtr ptr );
	std::list< CharacterPtr > getChara( );
	
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

