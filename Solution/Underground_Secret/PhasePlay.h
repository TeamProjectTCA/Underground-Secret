#pragma once
#include "Phase.h"
#include "const.h"
#include "smart_ptr.h"
#include <list>
#include <vector>
#include <unordered_map>

PTR( PhasePlay );
PTR( Character );
PTR( Drawer );
PTR( Sound );
PTR( Scroll );
PTR( Profiling );



class PhasePlay : public Phase {
public:
	PhasePlay( std::list< CharacterPtr > &chara, ScrollPtr _scroll );
	virtual ~PhasePlay( );

public:
	void update( );
	void draw( ) const;

private:
	bool isClear( ) const;
	bool isInvasion( ) const;

private:
	void drawTime( ) const;

private:
	std::list< CharacterPtr > _chara;

	int _time_count;
	int _chara_handle;

	std::unordered_map< SOUND, int > _sound_handle;

	DrawerPtr _drawer;
	SoundPtr _sound;
	ScrollPtr _scroll;
	ProfilingPtr _profiling;
	CharacterPtr _spy;
};

