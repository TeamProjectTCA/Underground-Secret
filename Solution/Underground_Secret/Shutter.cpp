#include "Shutter.h"
#include "Drawer.h"
#include "Mouse.h"
#include "const.h"

Shutter::Shutter( ) {
	_drawer = Drawer::getTask( );
	_mouse = Mouse::getTask( );

	_shutter_handle = _drawer->getImage( "shutter" );
	_shutter_width = _drawer->getImageWidth( "shutter" );
	_shutter_height = _drawer->getImageHeight( "shutter" );
}

Shutter::~Shutter( ) {
}

void Shutter::update( ) {
	action( );
}

void Shutter::draw( int col, Vector scroll ) const {
	const int SHUTTER_MAX = ( int )_shutter.size( );
	for ( int i = 0; i < SHUTTER_MAX; i++ ) {
 		float x = ( float )( _shutter[ i ].front( ) % col ) * BLOCK_SIZE;
		float y = ( float )( _shutter[ i ].front( ) / col ) * BLOCK_SIZE;
		int width = _shutter_width;
		int height = _shutter_height;

		if ( _shutter_state[ i ] == SHUTTER_STATE_NON_ACTIVE ) {
			height = 0;
		}

		// ƒXƒNƒ[ƒ‹•ª‚ð‘«‚·
		x += ( float )scroll.x * BLOCK_SIZE;
		y += ( float )scroll.y * BLOCK_SIZE;

		_drawer->drawRectGraph( x, y, 0, 0, width, height, _shutter_handle, true );
	}
}

void Shutter::action( ) {

}

void Shutter::addShutter( std::vector< int > shutter ) {
	_shutter.push_back( shutter );
	_shutter_state.push_back( SHUTTER_STATE_NON_ACTIVE );
}
