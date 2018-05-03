#pragma once
#include "smart_ptr.h"

PTR( Character );

enum CHARACTER {
	CHARA_A,
	CHARA_MAX,
};

class Character {
public:
	Character( );
	virtual ~Character( );

public:
	virtual void update( ) = 0;
};
