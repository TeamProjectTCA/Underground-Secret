#pragma once
#include "Character.h"
#include "Vector.h"

PTR( CharaDummy );
PTR( Map );
PTR( Infomation );

class CharaDummy : public Character {
public:
	CharaDummy( MapPtr map, InfomationPtr info );
	virtual ~CharaDummy( );

public:
	void update( );

private:
	void walk( );
	void fall( );
	void setDistance( );
	void countLooking( );
	void checkCollider( );
	void checkElevator( );

private:
	int _looking_time;
	MOVE_DIRECTION _dir;
	Vector _distance;
	bool _ride_elevator;
};

