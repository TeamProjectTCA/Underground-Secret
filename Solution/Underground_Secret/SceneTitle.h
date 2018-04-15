#pragma once
#include "Scene.h"
#include "Collider.h"
#include "Image.h"
#include "smart_ptr.h"
#include <string>
#include <array>
#include <Windows.h>

PTR( SceneTitle );
PTR( Drawer );
PTR( Keyboard );
PTR( Mouse );

class SceneTitle : public Scene {
private:
	enum BUTTON {
		NONE_BUTTON,
		START_BUTTON,
		BUTTON_MAX,
	}; 
	struct BoxObject {
		Image::ImageProperty image;
		BoxCollider collider;
	};
public:
	SceneTitle( );
	virtual ~SceneTitle( );

public:
	void initialize( );
	void finalize( );
	void update( );

private:
	void changeNextScene( );
	void calcButtonAction( );
	BUTTON getHitButton( ) const;
	
private:
	void drawBackGround( ) const;
	void drawTitle( ) const;
	void drawStartButton( ) const;

private:
	
	//ïœêî
	bool _startbutton_clicking;
	HCURSOR _cur_hand;

	int _background_handle;
	int _title_handle;
	std::array< int, BUTTON_MAX > _startbutton_handle;
	BoxObject _startbutton;


	//ÉNÉâÉX
	DrawerPtr _drawer;
	MousePtr _mouse;
	KeyboardPtr _keyboard;
};

