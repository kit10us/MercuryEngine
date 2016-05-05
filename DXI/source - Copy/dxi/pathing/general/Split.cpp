// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/pathing/general/Split.h>

using namespace dxi;
using namespace pathing;
using namespace general;

Split::Split( Modifier::shared_ptr modifierA, Modifier::shared_ptr modifierB, float unitTimeSpentOnA )
: m_modifierA( modifierA ), m_modifierB( modifierB ), m_unitTimeSpentOnA( unitTimeSpentOnA )
{
}

void Split::AtTime( float unitTime, unify::Frame * frame )
{
	if( unitTime <= m_unitTimeSpentOnA )
	{
		m_modifierA->AtTime( unitTime / m_unitTimeSpentOnA, frame );
	}
	else
	{
		m_modifierB->AtTime( (unitTime - m_unitTimeSpentOnA) / (1.0f - m_unitTimeSpentOnA), frame ); 
	}
}
