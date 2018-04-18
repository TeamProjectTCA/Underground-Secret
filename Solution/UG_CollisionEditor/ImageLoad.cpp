#include "ImageLoad.h"
#include "Keyboard.h"
#include "Drawer.h"
#include <string>
#include <array>

ImageLoad::ImageLoad( ) {
}

ImageLoad::~ImageLoad( ) {
}

void ImageLoad::actionEnter( ) {
	_handle = _drawer->getImage( _input );
	if( _handle == -1 ){
		return;
	}

	_fin = true;
}

std::string ImageLoad::getImageName( ) const {
	return _input;
}