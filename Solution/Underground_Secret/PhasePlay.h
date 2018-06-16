#pragma once
#include "Phase.h"
#include "smart_ptr.h"
#include <list>

PTR( PhasePlay );
PTR( Character );
PTR( Drawer );
PTR( Sound );
PTR( Scroll );
PTR( Profiling );

class PhasePlay : public Phase {
public:
	PhasePlay( std::list< CharacterPtr > &chara, int spy_idx, ScrollPtr _scroll );
	virtual ~PhasePlay( );

public:
	void update( );
	void draw( ) const;

private:
	bool isClear( );
	bool isInvasion( ) const;

private:
	void drawTime( ) const;

private:
	std::list< CharacterPtr > _chara;

	int _time_count;
	int _chara_handle;

	int _play_bgm;
	int _win_se;
	int _lose_se;

	DrawerPtr _drawer;
	SoundPtr _sound;
	ScrollPtr _scroll;
	ProfilingPtr _profiling;
	CharacterPtr _spy;
};

