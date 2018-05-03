#pragma once
#include "smart_ptr.h"

PTR( CharacterManager );
PTR( CharaA );

class CharacterManager {
private:
	enum CHARACTER {
		CHARA_A,
		CHARA_MAX,
	};

public:
	CharacterManager( );
	virtual ~CharacterManager( );

public:
	void initialize( );
	void finalize( );
	void update( );
	
	CharaAPtr _charaA;
};

