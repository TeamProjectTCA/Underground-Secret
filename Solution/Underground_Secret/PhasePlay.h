#pragma once
#include "Phase.h"
#include "smart_ptr.h"
#include <list>

PTR( PhasePlay );
PTR( Character );
PTR( Drawer );
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
	void countClear( );
	void setEnd( );

private:
	void drawTime( ) const;

private:
	std::list< CharacterPtr > _chara;
	const int FINISH_TIME = 60;

	int _time_count;
	int _chara_handle;

	DrawerPtr _drawer;
	ScrollPtr _scroll;
	ProfilingPtr _profiling;
};

