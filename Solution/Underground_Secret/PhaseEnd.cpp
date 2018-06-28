#include "PhaseEnd.h"
#include "Character.h"
#include "Map.h"
#include "Scroll.h"
#include "Sound.h"
#include "SpecialScroll.h"\
#include "const.h"

const float MOVE_FRAME = FPS * 3;
const float FOCUS_FRAME = FPS * 1;
const float PERFORMANCE_TIME = FPS * 2;
const float WIN_SE_ADJUST_TIME = FPS * 4;
const float LOSE_SE_ADJUST_TIME = FPS * 2;

const double MAP_SCROLL_SPEED_RATE = 0.7;
const double CHARA_SCROLL_SPEED_RATE = 1 - MAP_SCROLL_SPEED_RATE;
Vector CENTRAL = Vector( WIDTH_F / 2, HEIGHT_F / 2 );

PhaseEnd::PhaseEnd( std::list< CharacterPtr > &chara, MapPtr map, RESULT result, SpecialScrollPtr scroll ) :
_map( map ),
_count( 0 ),
_result( result ),
_end_scroll( scroll ) {
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
		_endpoint = _map->getFixedpointBeta( PHASE_END );
		_spy->setFixedpoint( PHASE_END );
		const float SPEED = ( float )( _endpoint - _spy->getPos( ) ).getLength( ) * ( 1 / MOVE_FRAME );
		_move = ( _endpoint - _spy->getPos( ) ).normalize( ) * SPEED;
	} else {
		_sound->play( _sound_handle[ WIN_SE ] );
	}
	
	_spy->setAnimTime( FPS );

	_scroll = _map->getScrollData( ); 
	const float FOCUS_SPEED = ( float )( ( CENTRAL - _scroll - _spy->getPos( ) ).getLength( ) * ( 1 / FOCUS_FRAME ) );
	_focus_move = ( CENTRAL - _scroll - _spy->getPos( ) ).normalize( ) * FOCUS_SPEED;
}

PhaseEnd::~PhaseEnd( ) {
}

void PhaseEnd::update( ) {
	_focus_count++;
	if ( _focus_count < FOCUS_FRAME ) {
		_spy->move( _focus_move );
		_end_scroll->setScroll( _focus_move );
		
		return;
	}
	if ( _result == LOSE ) {
		_count++;
		//定点まで移動
		if ( _count < MOVE_FRAME ) {
			if ( _map->getMapPos( ).x > 0 ||
				 _map->getMapPos( ).x < WIDTH - _map->getCol( ) * BLOCK_SIZE ||
				 _map->getMapPos( ).y < HEIGHT - _map->getRow( ) * BLOCK_SIZE ) {
				_spy->move( _move );
			} else {
				_end_scroll->setScroll( _move * MAP_SCROLL_SPEED_RATE * -1 );
				_spy->move( _move * CHARA_SCROLL_SPEED_RATE );
			}
			return;
		}

		if ( _count == MOVE_FRAME ) {
			// アニメーション
			_spy->setAnim( Character::ANIM_RIDE );
		}
		if ( _count == MOVE_FRAME + PERFORMANCE_TIME + LOSE_SE_ADJUST_TIME ) {
			setPhase( PHASE_RETURN_TITLE );
		}
	} else {
		if ( _focus_count == FOCUS_FRAME ) {
			// アニメーション
			_spy->setAnim( Character::ANIM_RIDE );
		}
		if ( _focus_count == FOCUS_FRAME + PERFORMANCE_TIME + WIN_SE_ADJUST_TIME ) {
			setPhase( PHASE_RETURN_TITLE );
		}
	}

	
}

void PhaseEnd::draw( ) const {
	_spy->draw( );
}