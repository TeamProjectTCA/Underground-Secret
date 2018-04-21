#include "FileDataSave.h"
#include "Drawer.h"
#include "const.h"
#include <assert.h>
#include <errno.h>
#include <vector>
#include <string>

const int WAIT_TIME = 60;

FileDataSave::FileDataSave( char* data, int col, int row ) :
FileAction( "FileDataSave" ),
_col( col ),
_row( row ),
_data( data ) {
	_drawer = Drawer::getTask( );
	_frame_handle = _drawer->getImage( "frame" );
	_gauge_handle = _drawer->getImage( "gauge" );
	_frame_width = _drawer->getImageWidth( "frame" );
	_frame_height = _drawer->getImageHeight( "frame" );
	_cnt = 0;
}

FileDataSave::~FileDataSave( ) {
}

void FileDataSave::actionEnter( ) {
	FILE *fp;
	errno_t file_open = fopen_s( &fp, ( "Import/Data/" + _input ).c_str( ), "w" );
	assert( file_open == 0 );

	std::vector< std::string > data;
	std::string input = "";
	long long int cnt = 0;
	while ( true ) {
		if ( _data[ cnt ] == '\0' ) {
			break;
		}

		input += _data[ cnt ];

		if ( cnt % _col == _col - 1 ) {
			data.push_back( input );
			std::string( ).swap( input );
		}

		cnt++;
	}
	
	int size = ( int )data.size( );
	for ( int i = 0; i < size; i++ ) {
		fprintf_s( fp, ( data[ i ] + '\n' ).c_str( ) );
	}

	fclose( fp );
	_enter = false;
	setGauge( );
}

void FileDataSave::upGauge( ) {
	const float X = WIDTH / 2;
	const float Y = HEIGHT / 3 * 2;
	const float MAX_X = ( X + _frame_width / 2 ) - ( X - _frame_width / 2 );
	const float RATE_X = MAX_X / WAIT_TIME;

	//ƒQ[ƒW‚Ì•`‰æ
	_drawer->drawExtendGraph( X - _frame_width / 2, Y - _frame_height / 2,
							  X - _frame_width / 2 + RATE_X * _cnt, Y + _frame_height / 2,
							 _gauge_handle, true );

	//ƒtƒŒ[ƒ€‚Ì•`‰æ
	_drawer->drawRotaGraph( X, Y, 1, 0, _frame_handle, true );

	if ( _cnt < WAIT_TIME ) {
		_cnt++;
	} else {
		_fin = true;
	}
}
