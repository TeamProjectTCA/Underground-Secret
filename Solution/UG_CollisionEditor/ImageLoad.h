#pragma once
#include "FileAction.h"
#include <string>

PTR( ImageLoad );

class ImageLoad : public FileAction {
public:
	ImageLoad( );
	virtual ~ImageLoad( );

public:
	void actionEnter( );
	int getImageHandle( ) const;
	int getImageWidth( ) const;
	int getImageHeight( ) const;

private:
	int _handle;
	int _width;
	int _height;
};

