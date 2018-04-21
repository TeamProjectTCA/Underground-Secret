#include "FileExport.h"
#include <assert.h>
#include <errno.h>
#include <vector>
#include <string>

FileExport::FileExport( char* data, int col, int row ) :
FileAction( "FileExport" ),
_col( col ),
_row( row ) {
	_enter = true;
	_data = data;
}

FileExport::~FileExport( ) {

}

void FileExport::actionEnter( ) {
	FILE *fp;
	errno_t file_open = fopen_s( &fp, "Export/collision_data.col", "w" );
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
	_fin = true;
}
