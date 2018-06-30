#pragma once
#include "Scene.h"
#include "smart_ptr.h"
#include <vector>

PTR( SceneManual );
PTR( Keyboard );
PTR( Drawer );
PTR( Mouse );
PTR( Button );

class SceneManual : public Scene {
public:
	SceneManual( );
	virtual ~SceneManual( );

public:
	void initialize( );
	void update( );
	void draw( ) const;

private:
	//ƒNƒ‰ƒX
	DrawerPtr _drawer;
	MousePtr _mouse;
	KeyboardPtr _keyboard;
	ButtonPtr _button;
	ButtonPtr _back_button;
};