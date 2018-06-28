#pragma once
#include "Phase.h"
#include "smart_ptr.h"
#include "Vector.h"
#include "const.h"
#include <unordered_map>
#include <list>

PTR( PhaseEnd );
PTR( Character );
PTR( Map );
PTR( Sound );
PTR( SpecialScroll );
PTR( SpecialElevator );

class PhaseEnd : public Phase {
public:
	PhaseEnd( std::list< CharacterPtr > &chara, MapPtr map, RESULT result, SpecialScrollPtr scroll );
	virtual ~PhaseEnd( );

public:
	void update( );
	void draw( ) const;
	
public:
	void updateLose( );
	void updateWin( );

private:
	Vector _endpoint;
	Vector _scroll;
	Vector _move;
	Vector _focus_move;
	int _count;
	int _focus_count;
	RESULT _result;

	std::unordered_map< SOUND, int > _sound_handle;

	SoundPtr _sound;
	CharacterPtr _spy;
	MapPtr _map;
	SpecialScrollPtr _end_scroll;
	SpecialElevatorPtr _elevator;
};

