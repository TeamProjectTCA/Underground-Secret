#pragma once
#include "smart_ptr.h"

PTR( CharacterManager );

class CharacterManager {


public:
	CharacterManager( );
	virtual ~CharacterManager( );

public:
	void initialize( );
	void finalize( );
	void update( );
};

