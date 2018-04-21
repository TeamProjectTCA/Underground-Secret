#pragma once
#include "FileAction.h"
#include "smart_ptr.h"

PTR( FileDataLoad );

class FileDataLoad : public FileAction {
public:
	FileDataLoad( );
	virtual ~FileDataLoad( );

public:
	void actionEnter( );
};

