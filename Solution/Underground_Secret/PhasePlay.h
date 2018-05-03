#pragma once
#include "Phase.h"
#include "smart_ptr.h"

PTR( PhasePlay );
PTR( Drawer );

class PhasePlay : public Phase {
public:
	PhasePlay( );
	virtual ~PhasePlay( );

public:
	void update( );

private:
	void countClear( );

private:
	void drawTime( );

private:
	const int FINISH_TIME = 60;

	int _time_count;
	int _chara_handle;

	DrawerPtr _drawer;
};

