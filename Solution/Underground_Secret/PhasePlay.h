#pragma once
#include "Phase.h"
#include "smart_ptr.h"
#include <list>

PTR( PhasePlay );
PTR( Character );
PTR( Drawer );

class PhasePlay : public Phase {
public:
	PhasePlay( std::list< CharacterPtr > &chara );
	virtual ~PhasePlay( );

public:
	void update( );

private:
	void countClear( );
	void setEnd( );

private:
	void drawTime( );

private:
	std::list< CharacterPtr > _chara;
	const int FINISH_TIME = 60;

	int _time_count;
	int _chara_handle;

	DrawerPtr _drawer;
};

