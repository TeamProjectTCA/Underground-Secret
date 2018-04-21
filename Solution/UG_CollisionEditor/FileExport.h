#pragma once
#include "smart_ptr.h"
#include "FileAction.h"

PTR( FileExport );
PTR( Drawer );

class FileExport : public FileAction {
public:
	FileExport( char* data, int col, int row );
	virtual ~FileExport( );

public:
	void actionEnter( );
	void upGauge( );

private:
	char* _data;
	const int _col;
	const int _row;

	int _frame_handle;
	int _gauge_handle;
	int _frame_width;
	int _frame_height;
	int _cnt;

	DrawerPtr _drawer;
};

