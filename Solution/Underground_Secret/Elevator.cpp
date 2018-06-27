#include "Elevator.h"
#include "const.h"
#include "Random.h"
#include "Drawer.h"
#include "Sound.h"

const char ELEVATOR_IMAGE[ ] = "elevator";
const char ELEVATOR_BACK_IMAGE[ ] = "elevator_back";
const float ELEVATOR_ANIM_OPEN = 30;

const int ELEVATOR_TOTAL_TIME = FPS * 20;
const int ELEVATOR_RIDE_TIME = FPS;
const int ELEVATOR_MOVE_TIME = FPS;
const int ELEVATOR_ANIM_TIME = ELEVATOR_RIDE_TIME + ELEVATOR_MOVE_TIME;
const int ELEVATOR_WAIT_TIME = ELEVATOR_TOTAL_TIME / 3 - ELEVATOR_ANIM_TIME; // エレベーター受付時間
const int ELEVATOR_ANIMFRAME_MAX = ELEVATOR_RIDE_TIME;
const int ONE_ELEVATOR_TOTAL_TIME = ELEVATOR_TOTAL_TIME / 3;

Elevator::Elevator( const int col, const char id ) :
_col( col ),
_id( id ),
_next_input_pos( ELEVATOR_POS_UP ),
_active_elevator( ELEVATOR_POS_UP ),
_elevator_state( ELEVATOR_STATE_NONE ),
_destination( ELEVATOR_POS_UP ) {
	RandomPtr random = Random::getTask( );
	int start = random->getInt32( 0, ELEVATOR_POS_MAX - 1 );
	_count = ( start * ( ELEVATOR_WAIT_TIME ) ) % ELEVATOR_TOTAL_TIME;
	_active_elevator = ( ELEVATOR_POS )( start % ELEVATOR_POS_MAX );
	decideDestination( );

	_drawer = Drawer::getTask( );
	_back_image = _drawer->getImage( ELEVATOR_BACK_IMAGE );
}

Elevator::~Elevator( ) {
}

void Elevator::update( ) {
	_count = ( _count + 1 ) % ELEVATOR_TOTAL_TIME;

	// active elevator の更新
	// destination の更新
	updateActiveElevator( );

	// elevator state の更新
	updateElevatorState( );

	// elevator anim の更新
	updateElevatorAnim( );

	//SE
	playSoundEffect( );
}

void Elevator::draw( ) const {
	for ( int i = 0; i < _elevator_anim.size( ); i++ ) {
		int x = ( _data[ i ] % _col ) * BLOCK_SIZE + ( int )_scroll.x;
		int y = ( _data[ i ] / _col ) * BLOCK_SIZE + ( int )_scroll.y;

		int width  = _drawer->getImageWidth ( ELEVATOR_IMAGE );
		int height = _drawer->getImageHeight( ELEVATOR_IMAGE );

		float draw_x = ( float )( x + BLOCK_SIZE / 2 );
		float draw_y = ( float )( y - height + BLOCK_SIZE );
		float draw_lx = draw_x - width;
		float draw_rx = draw_x;
		int handle = _drawer->getImage( ELEVATOR_IMAGE );

		if ( i == ( int )_active_elevator ) {
			if ( _elevator_state == ELEVATOR_STATE_COME ) {
				float move = ( float )sin( ( PI / ELEVATOR_ANIMFRAME_MAX ) * _elevator_anim[ ( int )_active_elevator ] ) * ELEVATOR_ANIM_OPEN;
				draw_lx -= move;
				draw_rx += move;
			}
		}

		if ( i == ( int )_destination ) {
			if ( _elevator_state == ELEVATOR_STATE_MOVE || _elevator_state == ELEVATOR_STATE_ARRIVE ) {
				float move = ( float )sin( ( PI / ELEVATOR_ANIMFRAME_MAX ) * _elevator_anim[ ( int )_destination ] ) * ELEVATOR_ANIM_OPEN;
				draw_lx -= move;
				draw_rx += move;
			}
		}

		_drawer->drawGraph( draw_x - width, draw_y, _back_image, true );
		_drawer->drawGraph( draw_lx, draw_y, handle, true );
		_drawer->drawGraph( draw_rx, draw_y, handle, true );
	}
}

void Elevator::updateActiveElevator( ) {
	if ( _count % ( ONE_ELEVATOR_TOTAL_TIME ) == 0 ) {

		if ( _data.size( ) < ELEVATOR_POS_MAX ) {
			_active_elevator = ( ELEVATOR_POS )( ( _active_elevator + 1 ) % ( int )_data.size( ) );
		} else {
			_active_elevator = ( ELEVATOR_POS )( ( _active_elevator + 1 ) % ELEVATOR_POS_MAX );
		}
		decideDestination( );
		resetAnimCount( );
	}
}

void Elevator::updateElevatorState( ) {
	int wait = _count % ( ONE_ELEVATOR_TOTAL_TIME );
	if ( wait < ELEVATOR_WAIT_TIME ) {
		_elevator_state = ELEVATOR_STATE_WAIT;
	} 
	if ( ELEVATOR_WAIT_TIME < wait ) {
		_elevator_state = ELEVATOR_STATE_COME;
	}
	if ( ELEVATOR_WAIT_TIME + ELEVATOR_RIDE_TIME < wait ) {
		_elevator_state = ELEVATOR_STATE_MOVE;
	}

	if ( ONE_ELEVATOR_TOTAL_TIME - ELEVATOR_MOVE_TIME / 2 < wait ) {
		_elevator_state = ELEVATOR_STATE_ARRIVE;
	}
}

void Elevator::updateElevatorAnim( ) {
	for ( int i = 0; i < _elevator_anim.size( ); i++ ) {
		if ( _elevator_anim[ i ] < ELEVATOR_ANIMFRAME_MAX ) {
			if ( i == ( int )_active_elevator && _elevator_state == ELEVATOR_STATE_COME ) {
				_elevator_anim[ i ]++;
			}

			if ( i == ( int )_destination && ( _elevator_state == ELEVATOR_STATE_MOVE || _elevator_state == ELEVATOR_STATE_ARRIVE ) ) {
				_elevator_anim[ i ]++;
			}
		}
	}
}

void Elevator::add( int idx ) {
	if ( _next_input_pos == ELEVATOR_POS_MAX ) {
		return;
	}

	_data.push_back( idx );
	_elevator_anim.push_back( 0 );
	_next_input_pos = ( ELEVATOR_POS )( _next_input_pos + 1 );
}

void Elevator::setScroll( Vector scroll ) {
	_scroll = scroll;
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

void Elevator::resetAnimCount( ) {
	for ( int i = 0; i < _elevator_anim.size( ); i++ ) {
		_elevator_anim[ i ] = 0;
	}
}

void Elevator::playSoundEffect( ) {
	for ( int i = 0; i < _elevator_anim.size( ); i++ ) {
		int x = ( _data[ i ] % _col ) * BLOCK_SIZE + ( int )_scroll.x;
		int y = ( _data[ i ] / _col ) * BLOCK_SIZE + ( int )_scroll.y;

		DrawerPtr drawer = Drawer::getTask( );
		SoundPtr sound = Sound::getTask( );
		int height = drawer->getImageHeight( ELEVATOR_IMAGE );
		
		float elevator_cx = ( float )( x + BLOCK_SIZE / 2 );
		float elevator_cy = ( float )( y - height / 2 );
		_sound_handle[ ELEVATOR_COME_SE ] = sound->load( "SoundEffect/ElevatorArrive1.ogg" );
		_sound_handle[ ELEVATOR_ARRIVE_SE ] = sound->load( "SoundEffect/ElevatorArrive2.ogg" );

		if ( i == ( int )_active_elevator ) {
			if ( _elevator_state == ELEVATOR_STATE_COME ) {
				if ( 0 < elevator_cx && elevator_cx < WIDTH &&
					 0 < elevator_cy && elevator_cy < HEIGHT ) {
					sound->play( _sound_handle[ ELEVATOR_COME_SE ] );
				}
			}
		}

		if ( i == ( int )_destination ) {
			if ( _elevator_state == ELEVATOR_STATE_MOVE ) {
				if ( 0 < elevator_cx && elevator_cx < WIDTH &&
					 0 < elevator_cy && elevator_cy < HEIGHT ) {
					sound->play( _sound_handle[ ELEVATOR_ARRIVE_SE ] );
				}
			}
		}
	    //debug
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


ELEVATOR_POS Elevator::getDestination( ) const {
	return _destination;
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