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
	Character( MapPtr map );
	virtual ~Character( );

public:
	// �V����Animation��ǉ�
	void addAnim( ANIM_TYPE type, std::string filename, int frame_num );
	// �A�j���[�V������؂�ւ���
	void setAnim( ANIM_TYPE type );
	void setAnimTime( int change_time );
	void setFixedpoint( PHASE phase );

public:
	// �ړ�����
	void move( Vector move );

public:
	Vector getPos( ) const;
	int getMapData( Vector pos ) const;

public:
	void draw( );
	virtual void update( ) = 0;

protected:
	void setPos( Vector pos );

protected:
	MapPtr _map;

private:
	Vector _pos;
	std::map< ANIM_TYPE, Animation > _anim;
	int _anim_change_time;
	int _max_cnt;
	int _cnt;
	int _sx;
	Vector _scroll;
	ANIM_TYPE _anim_type;

	DrawerPtr _drawer;
};
