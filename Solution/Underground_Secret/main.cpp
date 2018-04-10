#include "Manager.h"
#include "DxSetting.h"
#include "Drawer.h"

void main( ) {
	ManagerPtr manager = Manager::getInstance( );
	DxSettingPtr setting = DxSetting::getInstance( );
	setting->changeWindowMode( true );
	setting->setGraphMode( 1280, 720, 32 );
	setting->setWindowSize( 1280, 720 );
	setting->setDrawScreenBack( );

	DrawerPtr drawer( new Drawer( "Resources/image" ) );

	//add
	manager->add( Drawer::getTag( ), drawer );
}