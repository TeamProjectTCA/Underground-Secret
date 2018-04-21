#pragma once
#include "FileAction.h"
#include "smart_ptr.h"

PTR( FileDataSave );

class FileDataSave : public FileAction {
public:
	FileDataSave( );
	virtual ~FileDataSave( );

public:
	void actionEnter( );
};

