#pragma once
#include "Base.h"
#include <string>

PTR( CharacterManager );

class CharacterManager : public Base {
public:
	static CharacterManagerPtr getTask( );
	static std::string getTag( );

public:
	CharacterManager( );
	virtual ~CharacterManager( );

public:
	void initialize( );
	void finalize( );
	void update( );
};

