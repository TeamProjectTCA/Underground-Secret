#include "EditorManager.h"
#include "Manager.h"
#include "Keyboard.h"
#include "FileImageLoad.h"
#include "FileDataLoad.h"
#include "FileExport.h"
#include "FileDataSave.h"
#include "Table.h"
#include "const.h"
#include "Drawer.h"
#include <array>

const int ACTION_IMAGE_LOAD_NUM = 1;
const int ACTION_DATA_LOAD_NUM  = 2;
const int ACTION_EXPORT_NUM     = 5;
const int ACTION_DATA_SAVE_NUM  = 6;
const int ACTION_CANCEL_NUM     = 8;

EditorManagerPtr EditorManager::getTask( ) {
	return std::dynamic_pointer_cast< EditorManager >( Manager::getInstance( )->getTask( getTag( ) ) );
}

std::string EditorManager::getTag( ) {
	return "EDITOR_MANAGER";
}

EditorManager::EditorManager( ) {
	_command = COMMAND_NONE;
}

EditorManager::~EditorManager( ) {
}

void EditorManager::initialize( ) {
	_keyboard = Keyboard::getTask( );
	_action = FileImageLoadPtr( new FileImageLoad( ) );
	_table = TablePtr( new Table( -1, 1, 1 ) );
}

void EditorManager::finalize( ) {
}

void EditorManager::update( ) {
	if ( _command == COMMAND_NONE ) {
		_table->update( );
	} else {
		_action->update( );
	}

	//アクション解除
	if ( _keyboard->isKeyDownFunction( ACTION_CANCEL_NUM ) ) {
		_command = COMMAND_NONE;
		DrawerPtr drawer = Drawer::getTask( );
		drawer->flip( );
		return;
	}

	//コマンドをチェック
	std::array< int, COMMAND_MAX > command = { 
		ACTION_IMAGE_LOAD_NUM , 
		ACTION_DATA_LOAD_NUM  ,
		ACTION_EXPORT_NUM     ,
		ACTION_DATA_SAVE_NUM  ,
	};

	for ( int i = 0; i < COMMAND_MAX; i++ ) {
		if ( _keyboard->isKeyDownFunction( command[ i ] ) ) {
			_command = ( COMMAND )i;
			switch ( _command ) {
			case COMMAND_NONE:
				break;

			case COMMAND_IMAGE_LOAD:
				_action = FileImageLoadPtr( new FileImageLoad( ) );
				break;

			case COMMAND_DATA_LOAD:
				_action = FileDataLoadPtr( new FileDataLoad( ) );
				break;

			case COMMAND_DATA_EXPORT:
				{
					FileExportPtr file_export( new FileExport( _table->getData( ), _table->getCol( ), _table->getRow( ) ) );
					_action = file_export;
				}
				break;

			case COMMAND_DATA_SAVE:
				_action = FileDataSavePtr( new FileDataSave( ) );
				break;

			default:
				break;
			}
			break;
		}
	}

	if ( !_action->isFin( ) ) {
		return;
	}

	switch ( _command ) {
	case COMMAND_NONE:
		break;

	case COMMAND_IMAGE_LOAD:
		{
			FileImageLoadPtr image_load = std::dynamic_pointer_cast< FileImageLoad >( _action );
			_table->memoryFree( );
			_table = TablePtr( new Table( image_load->getImageHandle( ), image_load->getImageWidth( ) / BLOCK_SIZE, image_load->getImageHeight( ) / BLOCK_SIZE ) );
			_command = COMMAND_NONE;
		}
		break;

	case COMMAND_DATA_LOAD:
		{
			FileDataLoadPtr data_load = std::dynamic_pointer_cast< FileDataLoad >( _action );
			_table->memoryFree( );
			_table->loadCollider( data_load->getData( ), data_load->getCol( ), data_load->getRow( ) );
			_command = COMMAND_NONE;
		}
		break;

	case COMMAND_DATA_EXPORT:
		_command = COMMAND_NONE;
		break;

	default:
		break;
	}
}