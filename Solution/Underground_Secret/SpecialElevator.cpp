#include "SpecialElevator.h"
#include "Drawer.h"
#include "const.h"
#include "Scroll.h"

const int ANIM_COUNT_MAX = 60;
const int OPEN_DISTANCE = 30;
const float GAP_Y = 16;
const char ELEVATOR_BACK_IMAGE[ ] = "elevator_back";

SpecialElevator::SpecialElevator( ScrollConstPtr scroll ) :
_alpha( Data( ) ),
_beta ( Data( ) ),
_height( 0 ),
_width( 0 ),
_scroll( scroll ) {
	_drawer = Drawer::getTask( );
	_back_handle = _drawer->getImage( ELEVATOR_BACK_IMAGE );
}

SpecialElevator::~SpecialElevator( ) {
}

void SpecialElevator::update( ) {
	calcAlpha( );
	calcBeta( );
}

void SpecialElevator::calcAlpha( ) {
	if ( !_alpha.active ) {
		return;
	}
	if ( ANIM_COUNT_MAX < _alpha.cnt ) {
		return;
	}

	_alpha.cnt++;
}

void SpecialElevator::calcBeta( ) {
	if ( !_beta.active ) {
		return;
	}

	if ( ANIM_COUNT_MAX < _beta.cnt ) {
		return;
	}

	_beta.cnt++;
}

void SpecialElevator::draw( ) const {
	drawAlpha( );
	drawBeta( );
}

void SpecialElevator::drawAlpha( ) const {
	float draw_x = ( float )_alpha.pos.x;
	float draw_y = ( float )_alpha.pos.y - _height + GAP_Y;

	Vector scroll = _scroll->getScroll( );
	draw_x += ( float )scroll.x;
	draw_y += ( float )scroll.y;

	float move = ( float )sin( PI / ANIM_COUNT_MAX * _alpha.cnt ) * OPEN_DISTANCE;
	
	_drawer->drawGraph( draw_x - _width, draw_y, _back_handle, true );
	_drawer->drawGraph( draw_x - _width - move, draw_y, _handle, true );
	_drawer->drawGraph( draw_x          + move, draw_y, _handle, true );
}

void SpecialElevator::drawBeta( ) const {
	float draw_x = ( float )_beta.pos.x;
	float draw_y = ( float )_beta.pos.y - _height + GAP_Y;

	Vector scroll = _scroll->getScroll( );
	draw_x += ( float )scroll.x;
	draw_y += ( float )scroll.y;

	float move = ( float )sin( PI / ANIM_COUNT_MAX * _beta.cnt ) * OPEN_DISTANCE;

	_drawer->drawGraph( draw_x - _width, draw_y, _back_handle, true );
	_drawer->drawGraph( draw_x - _width - move, draw_y, _handle, true );
	_drawer->drawGraph( draw_x          + move, draw_y, _handle, true );
}

void SpecialElevator::setElevator( Vector start, Vector end ) {
	_alpha.pos = start;
	_beta.pos = end;
}

void SpecialElevator::setImage( const char *path ) {
	_handle = _drawer->getImage( path );
	_height = _drawer->getImageHeight( path );
	_width  = _drawer->getImageWidth( path );
}

void SpecialElevator::setActiveAlpha( bool active ) {
	_alpha.active = active;
}

void SpecialElevator::setActiveBeta( bool active ) {
	_beta.active = active;
}

bool SpecialElevator::isCloseAlpha( ) const {
	return ( _alpha.cnt == ANIM_COUNT_MAX ? true : false );
}

bool SpecialElevator::isCloseBeta( ) const {
	return ( _beta.cnt == ANIM_COUNT_MAX ? true : false );
}