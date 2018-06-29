#pragma once
#include "Scene.h"
#include "Collider.h"
#include "Image.h"
#include "const.h"
#include "smart_ptr.h"
#include <string>
#include <array>
#include <Windows.h>
#include <unordered_map>

PTR( SceneTitle );
PTR( Drawer );
PTR( Sound );
PTR( Keyboard );
PTR( Mouse );
PTR( Button );

class SceneTitle : public Scene {
private:
	enum BUTTON {
		BUTTON_NONE,
		BUTTON_START,
		BUTTON_MAX,
	}; 
	struct BoxObject {
		ImageProperty image;
		BoxCollider collider;
	};
public:
	SceneTitle( );
	virtual ~SceneTitle( );

public:
	void initialize( );
	void finalize( );
	void update( );
	void draw( ) const;

private:
	
	//ïœêî
	bool _startbutton_clicking;
	HCURSOR _cur_hand;

	int _background_image;
	int _title_image;

	std::unordered_map< SOUND, int > _sound_handle;

	std::array< int, BUTTON_MAX > _startbutton_handle;
	BoxObject _startbutton;


	//ÉNÉâÉX
	DrawerPtr _drawer;
	SoundPtr _sound;
	MousePtr _mouse;
	KeyboardPtr _keyboard;
	ButtonPtr _start_button;
	ButtonPtr _manual_button;
};

