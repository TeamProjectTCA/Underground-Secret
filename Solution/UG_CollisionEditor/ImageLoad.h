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
	std::string getImageName( ) const;

private:
	int _handle;
};

