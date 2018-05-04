#pragma once
#include "smart_ptr.h"
#include "Vector.h"
#include <string>
#include <map>
#include <vector>

PTR( Character );
PTR( Drawer );

enum CHARACTER {
	CHARA_A,
	CHARA_MAX,
};

class Character {
public:
	enum ANIM_TYPE {
		WALK,
	};

private:
	struct Animation {
		int handle;
		int frame;
		int width;
		int height;

		Animation( ) :
			handle( -1 ),
			frame( 1 ),
			width( 1 ),
			height( 1 ) {
		};

		Animation( int input_handle, int input_frame, int input_width, int input_height ) :
			handle( input_handle ),
			frame( input_frame ),
			width( input_width ),
			height( input_height ) {
		};
	};

public:
	Character( std::string walk_anim_filename, int walk_anim_frame_num );
	virtual ~Character( );

public:
	// 新しくAnimationを追加
	void addAnim( ANIM_TYPE type, std::string filename, int frame_num );
	// アニメーションを切り替える
	void setAnim( ANIM_TYPE type );
	void setAnimTime( int change_time );

public:
	void draw( );
	virtual void update( ) = 0;

protected:
	Vector _pos;

private:
	std::map< ANIM_TYPE, Animation > _anim;
	int _anim_change_time;
	int _max_cnt;
	int _cnt;
	int _sx;
	ANIM_TYPE _anim_type;

	DrawerPtr _drawer;
};
