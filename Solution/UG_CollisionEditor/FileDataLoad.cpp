#include "FileDataLoad.h"
#include <assert.h>
#include <errno.h>
#include <vector>

FileDataLoad::FileDataLoad( ) :
FileAction( "FileDataLoad" ) {
	_col = 0;
	_row = 0;
	_data = "";
}

FileDataLoad::~FileDataLoad( ) {
}

void FileDataLoad::actionEnter( ) {
	FILE *fp;
	if ( fopen_s( &fp, ( "Import/Data/" + _input ).c_str( ), "r" ) != 0 ) {
		return;	
	}

	int col = 0;
	int row = 0;
	while ( true ) {
		char buf[ 2048 ] = { };
		if ( fgets( buf, 2048, fp ) == NULL ) {
			break;
		}

		std::string str = buf;
		// \nÇè¡Ç∑
		str = str.substr( 0, str.length( ) - 1 );

		_data += str;
		col = ( int )str.length( );
		row++;
	}

	fclose( fp );
	_col = col;
	_row = row;
	_fin = true;
}

std::string FileDataLoad::getData( ) {
	return _data;
}

int FileDataLoad::getCol( ) const {
	return _col;
}

int FileDataLoad::getRow( ) const {
	return _row;
}
