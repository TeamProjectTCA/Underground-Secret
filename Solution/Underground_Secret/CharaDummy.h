#pragma once
#include "Character.h"
#include "Vector.h"

PTR( CharaDummy );
PTR( Map );

class CharaDummy : public Character {
public:
	CharaDummy( MapPtr map );
	virtual ~CharaDummy( );

public:
	void update( );

private:
	void walk( );
	void fall( );
	void setDistance( );
	void countLooking( );

private:
	int _looking_time;
	MOVE_DIRECTION _dir;
	Vector _distance;
};

