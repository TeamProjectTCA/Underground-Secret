#pragma once
#include "smart_ptr.h"
#include "FileAction.h"

PTR( FileExport );

class FileExport : public FileAction{
public:
	FileExport( char* data, int col, int row );
	virtual ~FileExport( );

public:
	void actionEnter( );

private:
	char* _data;
	const int _col;
	const int _row;
};

