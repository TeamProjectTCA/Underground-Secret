#pragma once
#include "FileAction.h"
#include "smart_ptr.h"

PTR( FileDataSave );
PTR( Drawer );

class FileDataSave : public FileAction {
public:
	FileDataSave( char* data, int col, int row );
	virtual ~FileDataSave( );

public:
	void actionEnter( );
	void upGauge( );

private:
	char *_data;
	const int _col;
	const int _row;

	int _frame_handle;
	int _gauge_handle;
	int _frame_width;
	int _frame_height;
	int _cnt;

	DrawerPtr _drawer;
};

