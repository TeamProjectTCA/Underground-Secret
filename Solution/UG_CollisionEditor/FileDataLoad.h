#pragma once
#include "FileAction.h"
#include "smart_ptr.h"
#include <string>

PTR( FileDataLoad );

class FileDataLoad : public FileAction {
public:
	FileDataLoad( );
	virtual ~FileDataLoad( );

public:
	void actionEnter( );

public:
	std::string getData( );
	int getCol( ) const;
	int getRow( ) const;

private:
	int _row;
	int _col;
	std::string _data;
};

