#pragma once
#include "Character.h"
#include "Vector.h"

PTR( CharaDummy );
PTR( Map );
PTR( Infomation );
PTR( Random );

class CharaDummy : public Character {
public:
	CharaDummy( MapPtr map, InfomationPtr info );
	virtual ~CharaDummy( );

public:
	void update( );

private:
	void walk( );
	void wait( );
	void fall( );
	void setDistance( );
	void countLooking( );
	void checkCollider( );
	void checkElevator( );
	void returnMove( );

private:
	int _looking_time;
	int _wait_count;
	int _wait_ani_time;
	int _wait_time;
	MOVE_DIRECTION _dir;
	Vector _distance;
	bool _ride_elevator;
	bool _return_move;

	RandomPtr _random;
};

