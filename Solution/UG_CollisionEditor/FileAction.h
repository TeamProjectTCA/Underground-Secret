#pragma once
#include "smart_ptr.h"
#include "Image.h"
#include <string>

PTR( MassCollider );
PTR( Keyboard );
PTR( Drawer );
PTR( Mouse );

class FileAction {
public:
	FileAction( );
	virtual ~FileAction( );

public:
	void update( );

protected:
	void writeName( );
	virtual void actionEnter( ) = 0;

private:
	void drawLoadedImage( ) const;
	void drawMass( ) const;
	void drawMassSelect( ) const;

protected:
	std::string _input;
	bool _enter;
	Image::ImageProperty _image;

	KeyboardPtr _keyboard;
	MousePtr _mouse;
	DrawerPtr _drawer;
	MassColliderPtr _mass_collider;
};

