#include "FileImageLoad.h"
#include "Keyboard.h"
#include "Drawer.h"
#include <string>
#include <array>

FileImageLoad::FileImageLoad( ) :
FileAction( "IMAGE_LOAD" ) {
	_handle = -1;
	_width = 0;
	_height = 0;
}

FileImageLoad::~FileImageLoad( ) {
}

void FileImageLoad::actionEnter( ) {
	_handle = _drawer->getImage( _input );
	if( _handle == -1 ){
		return;
	}
	_width = _drawer->getImageWidth( _input );
	_height = _drawer->getImageHeight( _input );

	_fin = true;
}

int FileImageLoad::getImageHandle( ) const {
	return _handle;
}

int FileImageLoad::getImageWidth( ) const {
	return _width;
}

int FileImageLoad::getImageHeight( ) const {
	return _height;
}