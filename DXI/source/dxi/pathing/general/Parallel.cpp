// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#include <dxi/pathing/general/Parallel.h>

using namespace dxi;
using namespace pathing;
using namespace general;

Parallel::Parallel( std::vector< Modifier::shared_ptr > modifiers )
: m_modifiers( modifiers )
{
}

void Parallel::AtTime( float unitTime, unify::Frame * frame )
{
	for( size_t i = 0, end = m_modifiers.size(); i != end; ++i )
	{
		m_modifiers[ i ]->AtTime( unitTime, frame );
	}
}
