#pragma once

#include "Display.h"

// Encapsulates a war display from the underlying display mechanics.
class WarDisplay : public Display
{
public:
	WarDisplay( const RECT & area );
	WarDisplay( const WarDisplay & display );
};