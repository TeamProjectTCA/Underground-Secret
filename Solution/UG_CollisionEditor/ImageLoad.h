#pragma once
#include "FileAction.h"

PTR( ImageLoad );

class ImageLoad : public FileAction {
public:
	ImageLoad( );
	virtual ~ImageLoad( );

public:
	void actionEnter( );
};

