#pragma once
#include "Character.h"

PTR( CharaDummy );
PTR( Map );

class CharaDummy : public Character {
public:
	CharaDummy( MapPtr map );
	virtual ~CharaDummy( );

public:
	void update( );
};

