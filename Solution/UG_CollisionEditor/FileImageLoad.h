#pragma once
#include "FileAction.h"
#include <string>

PTR( FileImageLoad );

class FileImageLoad : public FileAction {
public:
	FileImageLoad( );
	virtual ~FileImageLoad( );

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

