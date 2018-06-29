#pragma once
#include "smart_ptr.h"
#include "Vector.h"
#include "const.h"
#include <string>
#include <vector>
#include <unordered_map>

PTR( Elevator );
PTR( Drawer );

enum ELEVATOR_POS {
	ELEVATOR_POS_UP,
	ELEVATOR_POS_CENTER,
	ELEVATOR_POS_DOWN,
	ELEVATOR_POS_MAX
};

enum ELEVATOR_STATE {
	ELEVATOR_STATE_NONE,
	ELEVATOR_STATE_WAIT,
	ELEVATOR_STATE_COME,
	ELEVATOR_STATE_MOVE,
	ELEVATOR_STATE_ARRIVE,
	ELEVATOR_STATE_MAX
};

class Elevator {
public:
	Elevator( const int _col, const char id );
	virtual ~Elevator( );

public:
	void update( );
	void draw( ) const;

private:
	void updateActiveElevator( );
	void updateElevatorState( );
	void updateElevatorAnim( );

public:
	void add( int idx );
	void setScroll( Vector scroll );

private:
	void decideDestination( );
	void resetAnimCount( );
	void playSoundEffect( );

public:
	char getId( ) const;
	int getElevatorPos( ELEVATOR_POS pos ) const;
	int getDestinationPos( ) const;
	int getActivePos( ) const;
	ELEVATOR_STATE getElevatorState( ) const;
	ELEVATOR_POS getActiveElevator( ) const;
	ELEVATOR_POS getDestination( ) const;
	ELEVATOR_POS getElevatorPos( int idx ) const;

private:
	const int _col;
	const char _id;
	int _count;
	ELEVATOR_POS _next_input_pos;
	std::vector< int > _data;
	std::vector< int > _elevator_anim;
	Vector _scroll;
	ELEVATOR_POS _active_elevator;
	ELEVATOR_POS _destination;
	ELEVATOR_STATE _elevator_state;
	std::unordered_map< SOUND, int > _sound_handle;
	int _back_image;

	DrawerPtr _drawer;
};

