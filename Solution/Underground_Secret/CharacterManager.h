#pragma once
#include "smart_ptr.h"

PTR(CharacterManager);
PTR(Character);

class CharacterManager {


public:
	CharacterManager( );
	virtual ~CharacterManager( );

public:
	void initialize( );
	void finalize( );
	void update( );
	
private:
	CharacterPtr _character;
};

