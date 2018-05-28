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
PTR( Debug );

enum MOVE_DIRECTION {
	MOVE_RIGHT,
	MOVE_LEFT,
	MOVE_DOWN,
	MOVE_DIRECTION_MAX
};

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
	Character( MapPtr map, std::vector< std::string > info );
	virtual ~Character( );

public:
	// 新しくAnimationを追加
	void addAnim( ANIM_TYPE type, std::string filename, int frame_num );
	// アニメーションを切り替える
	void setAnim( ANIM_TYPE type );
	void setAnimTime( int change_time );
	void setFixedpoint( PHASE phase );
	void setScroll( );

public:
	// 移動処理
	void move( Vector move );

public:
	Vector getPos( ) const;
	int getMapDataCollider( Vector pos ) const;
	int getMapDataElevator( Vector pos ) const;
	bool isLooking( Vector pos ) const;
	bool isEndpoint( Vector pos ) const;

public:
	void draw( );
	virtual void update( ) = 0;

protected:
	void setFallPos( Vector now_position );
	void setPos( Vector pos );
	void setElevatorPos( int ascii );
	// コライダーに埋まってしまっているときの処理
	void checkCaughtCollider( );

protected:
	MapPtr _map;
	DrawerPtr _drawer;
	DebugPtr _debug;

private:
	bool _spy;
	Vector _pos;
	std::map< ANIM_TYPE, Animation > _anim;
	int _anim_change_time;
	int _anim_cnt;
	int _max_cnt;
	int _sx;
	Vector _scroll;
	ANIM_TYPE _anim_type;
	std::vector< std::string > _info;
};
