#pragma once
#include "smart_ptr.h"
#include "Vector.h"

PTR( SpecialScroll );
PTR( Scroll );

class SpecialScroll {
public:
	SpecialScroll( ScrollPtr scroll );
	virtual ~SpecialScroll( );

public:
	void setScroll( Vector vec );

private:
	ScrollPtr _scroll;
};

