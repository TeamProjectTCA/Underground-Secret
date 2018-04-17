#include "ImageLoad.h"
#include "Keyboard.h"
#include "Drawer.h"
#include <string>

ImageLoad::ImageLoad( ) {
}

ImageLoad::~ImageLoad( ) {
}

void ImageLoad::actionEnter( ) {
	_image.handle = _drawer->getImage( _input );
	_image.width = _drawer->getImageWidth( _input );
	_image.height = _drawer->getImageHeight( _input );
	_enter = true;
}