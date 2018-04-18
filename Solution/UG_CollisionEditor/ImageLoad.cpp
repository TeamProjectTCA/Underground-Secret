#include "ImageLoad.h"
#include "Keyboard.h"
#include "Drawer.h"
#include <string>
#include <array>

ImageLoad::ImageLoad( ) :
FileAction( "IMAGE_LOAD" ) {
	_handle = -1;
	_width = 0;
	_height = 0;
}

ImageLoad::~ImageLoad( ) {
}

void ImageLoad::actionEnter( ) {
	_handle = _drawer->getImage( _input );
	if( _handle == -1 ){
		return;
	}
	_width = _drawer->getImageWidth( _input );
	_height = _drawer->getImageHeight( _input );

	_fin = true;
}

int ImageLoad::getImageHandle( ) const {
	return _handle;
}

int ImageLoad::getImageWidth( ) const {
	return _width;
}

int ImageLoad::getImageHeight( ) const {
	return _height;
}