#include "SpecialScroll.h"
#include "Scroll.h"

SpecialScroll::SpecialScroll( ScrollPtr scroll ) :
_scroll( scroll ) {
}

SpecialScroll::~SpecialScroll( ) {
}

void SpecialScroll::setScroll( Vector vec ) {
	_scroll->setScroll( vec );
}