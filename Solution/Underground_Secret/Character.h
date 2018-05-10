#pragma once
#include "smart_ptr.h"
#include "Vector.h"
#include "Phase.h"
#include <string>
#include <map>
#include <vector>

PTR( Character );
PTR( Drawer );
PTR( Map );

class Character {
public:
	enum ANIM_TYPE {
		WALK,
		OPEN,
		ANIM_TYPE_MAX,
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
	Character( MapPtr map );
	virtual ~Character( );

public:
	// 新しくAnimationを追加
	void addAnim( ANIM_TYPE type, std::string filename, int frame_num );
	// アニメーションを切り替える
	void setAnim( ANIM_TYPE type );
	void setAnimTime( int change_time );
	void setFixedpoint( PHASE phase );

public:
	// 移動処理
	void moveX( double distance );

public:
	Vector getPos( ) const;

public:
	void draw( );
	virtual void update( ) = 0;

protected:
	Vector _pos;
	MapPtr _map;

private:
	std::map< ANIM_TYPE, Animation > _anim;
	int _anim_change_time;
	int _max_cnt;
	int _cnt;
	int _sx;
	Vector _scroll;
	ANIM_TYPE _anim_type;

	DrawerPtr _drawer;
};
