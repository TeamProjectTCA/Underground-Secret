#include "PhaseEnd.h"
#include "Character.h"
#include "Map.h"
#include "Scroll.h"
#include "Sound.h"
#include "const.h"

const float MOVE_FRAME = FPS * 3;
const float FOCUS_FRAME = FPS * 1;
const float PERFORMANCE_TIME = FPS * 2;
const float SE_ADJUST_TIME = FPS * 2;

const double MAP_SCROLL_SPEED_RATE = 0.7;
const double CHARA_SCROLL_SPEED_RATE = 1 - MAP_SCROLL_SPEED_RATE;
Vector CENTRAL = Vector( WIDTH_F / 2, HEIGHT_F / 2 );

PhaseEnd::PhaseEnd( std::list< CharacterPtr > &chara, MapPtr map, RESULT result ) :
_map( map ),
_count( 0 ),
_result( result ){
	//SE
	_sound = Sound::getTask( );
	_sound_handle[ WIN_SE ] = _sound->load( "SoundEffect/win.ogg" );
	_sound_handle[ LOSE_SE ] = _sound->load( "SoundEffect/lose.ogg" );
	
	std::list< CharacterPtr >::iterator ite;
	ite = chara.begin( );
	for ( ite; ite != chara.end( ); ite++ ) {
		if ( ( *ite )->isSpy( ) ) {
			break;
		}
	}
	_spy = ( *ite );

	if ( _result == LOSE ) {
		_sound->play( _sound_handle[ LOSE_SE ] );
		_spy->setFixedpoint( PHASE_END );
		_spy->setAnimTime( FPS );
		_endpoint = _map->getFixedpointBeta( PHASE_END );
		const float SPEED = ( float )( _endpoint - _spy->getPos( ) ).getLength( ) * ( 1 / MOVE_FRAME );
		_move = ( _endpoint - _spy->getPos( ) ).normalize( ) * SPEED;
	}
	if ( _result == WIN ) {
		_sound->play( _sound_handle[ WIN_SE ] );
	}

	_scroll = _map->getScrollData( ); 
	const float FOCUS_SPEED = ( float )( ( CENTRAL - _scroll - _spy->getPos( ) ).getLength( ) * ( 1 / FOCUS_FRAME ) );
	_focus_move = ( CENTRAL - _scroll - _spy->getPos( ) ).normalize( ) * FOCUS_SPEED;
}

PhaseEnd::~PhaseEnd( ) {
}

void PhaseEnd::update( ) {
	_focus_count++;
	if ( _focus_count <= FOCUS_FRAME ) {
		_spy->move( _focus_move );
		_map->endScroll( _focus_move * -1 );
		
		return;
	}

	_count++;
	//��_�܂ňړ�
	if ( _count < MOVE_FRAME ) {
		if ( _map->getMapPos( ).x > 0 ||
			 _map->getMapPos( ).x < WIDTH - _map->getCol( ) * BLOCK_SIZE ||
			 _map->getMapPos( ).y < HEIGHT - _map->getRow( ) * BLOCK_SIZE ) {
			_spy->move( _move );
		} else {
			_map->endScroll( _move * MAP_SCROLL_SPEED_RATE );
			_spy->move( _move * CHARA_SCROLL_SPEED_RATE );
		}
		return;
	}

	if ( _count == MOVE_FRAME ) {
		// �A�j���[�V����
		_spy->setAnim( Character::ANIM_RIDE );
	}

	if ( _count == MOVE_FRAME + PERFORMANCE_TIME + SE_ADJUST_TIME ) {
		setPhase( PHASE_RETURN_TITLE );
	}
	
}

void PhaseEnd::draw( ) const {
	_spy->draw( );
}