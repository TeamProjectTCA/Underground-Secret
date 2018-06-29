#pragma once
#include "Character.h"
#include "Vector.h"

PTR( Man1 );
PTR( Map );
PTR( Infomation );
PTR( Random );

class Man1 : public Character {
public:
	Man1( MapPtr map, InfomationPtr info );
	virtual ~Man1( );

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
	float _ride_probability;
	MOVE_DIRECTION _dir;
	Vector _distance;
	bool _ride_elevator;
	bool _return_move;
	bool _hit_shutter;
	bool _elevator_down;
	bool _judged_probability;

	RandomPtr _random;
};

