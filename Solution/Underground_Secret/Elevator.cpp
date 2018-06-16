#include "Elevator.h"
#include "const.h"
#include "Random.h"

const int ELEVATOR_TOTAL_TIME = FPS * 10;
const int ELEVATOR_MOVE_TIME = FPS; // エレベーター移動時間
const int ELEVATOR_WAIT_TIME = ELEVATOR_TOTAL_TIME / 3 - ELEVATOR_MOVE_TIME; // エレベーター受付時間

Elevator::Elevator( const char id ) :
_id( id ),
_next_input_pos( ELEVATOR_POS_UP ),
_active_elevator( ELEVATOR_POS_UP ),
_elevator_state( ELEVATOR_STATE_NONE ),
_destination( ELEVATOR_POS_UP ) {
	RandomPtr random = Random::getTask( );
	int start = random->getInt32( 0, ELEVATOR_POS_MAX - 1 );
	_count = ( start * ( ELEVATOR_WAIT_TIME + ELEVATOR_MOVE_TIME ) ) % ELEVATOR_TOTAL_TIME;
	_active_elevator = ( ELEVATOR_POS )( start % ELEVATOR_POS_MAX );
	decideDestination( );
}

Elevator::~Elevator( ) {
}

void Elevator::update( ) {
	_count = ( _count + 1 ) % ELEVATOR_TOTAL_TIME;
	int wait = _count % ( ELEVATOR_WAIT_TIME + ELEVATOR_MOVE_TIME );

	// active elevator の更新
	// destination の更新
	if ( _count % ( ELEVATOR_WAIT_TIME + ELEVATOR_MOVE_TIME ) == 0 ) {

		if ( _data.size( ) < ELEVATOR_POS_MAX ) {
			_active_elevator = ( ELEVATOR_POS )( ( _active_elevator + 1 ) % ( int )_data.size( ) );
		} else {
			_active_elevator = ( ELEVATOR_POS )( ( _active_elevator + 1 ) % ELEVATOR_POS_MAX );
		}
		decideDestination( );
	}

	// elevator state の更新
	if ( wait < ELEVATOR_WAIT_TIME ) {
		_elevator_state = ELEVATOR_STATE_WAIT;
	} else {
		_elevator_state = ELEVATOR_STATE_MOVE;
	}

	if ( wait == ( ELEVATOR_WAIT_TIME + ELEVATOR_MOVE_TIME - 1 ) ) {
		_elevator_state = ELEVATOR_STATE_ARRIVE;
	}
}

void Elevator::add( int idx ) {
	if ( _next_input_pos == ELEVATOR_POS_MAX ) {
		return;
	}

	_data.push_back( idx );
	_next_input_pos = ( ELEVATOR_POS )( _next_input_pos + 1 );
}

void Elevator::decideDestination( ) {
	switch ( _active_elevator ) {
	case ELEVATOR_POS_UP:
		_destination = ELEVATOR_POS_CENTER;
		break;

	case ELEVATOR_POS_CENTER:
	{
		RandomPtr random = Random::getTask( );
		random->setIgnore( ( unsigned long )ELEVATOR_POS_CENTER );
		_destination = ( ELEVATOR_POS )random->getInt32( ELEVATOR_POS_UP, ELEVATOR_POS_DOWN );
		random->resetIgnore( );
		break;
	}

	case ELEVATOR_POS_DOWN:
		_destination = ELEVATOR_POS_CENTER;
		break;
	}

	int size = ( int )_data.size( );
	if ( size < 1 ) {
		return;
	}
	if ( size < ELEVATOR_POS_MAX && _active_elevator < size ) {
		_destination = ( ELEVATOR_POS )( ( _active_elevator + 1 ) % size );
	}
}

char Elevator::getId( ) const {
	return _id;
}

ELEVATOR_STATE Elevator::getElevatorState( ) const {
	return _elevator_state;
}

ELEVATOR_POS Elevator::getActiveElevator( ) const {
	return _active_elevator;
}

int Elevator::getElevatorPos( ELEVATOR_POS pos ) const {
	if ( _data.size( ) <= ( int )pos ) {
		return -1;
	}

	return _data[ ( int )pos ];
}

int Elevator::getDestinationPos( ) const {
	return getElevatorPos( _destination );
}