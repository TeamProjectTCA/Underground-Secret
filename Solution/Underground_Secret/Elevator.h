#pragma once
#include "smart_ptr.h"
#include <string>
#include <vector>

PTR( Elevator );

enum ELEVATOR_POS {
	ELEVATOR_POS_UP,
	ELEVATOR_POS_CENTER,
	ELEVATOR_POS_DOWN,
	ELEVATOR_POS_MAX
};

enum ELEVATOR_STATE {
	ELEVATOR_STATE_NONE,
	ELEVATOR_STATE_WAIT,
	ELEVATOR_STATE_MOVE,
	ELEVATOR_STATE_ARRIVE,
	ELEVATOR_STATE_MAX
};

class Elevator {
public:
	Elevator( const char id );
	virtual ~Elevator( );

public:
	void update( );

public:
	void add( int idx );

private:
	void decideDestination( );

public:
	char getId( ) const;
	int getElevatorPos( ELEVATOR_POS pos ) const;
	int getDestinationPos( ) const;
	ELEVATOR_STATE getElevatorState( ) const;
	ELEVATOR_POS getActiveElevator( ) const;
	ELEVATOR_POS getDestination( ) const;


private:
	const char _id;
	ELEVATOR_POS _next_input_pos;
	std::vector< int > _data;
	ELEVATOR_POS _active_elevator;
	ELEVATOR_POS _destination;
	ELEVATOR_STATE _elevator_state;
	int _count;
};

