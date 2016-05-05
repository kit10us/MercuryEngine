// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/pathing/Modifier.h>	

namespace dxi
{
	namespace pathing
	{
		namespace general
		{
			// Time is split between two different modifiers based on factor.
			class Split : public Modifier
			{
			public:
				Split( Modifier::shared_ptr modifierA, Modifier::shared_ptr modifierB, float unitTimeSpentOnA = 0.5f );
				void AtTime( float unitTime, unify::Frame * frame );
			
			private:
				std::shared_ptr< Modifier > m_modifierA;
				std::shared_ptr< Modifier > m_modifierB;
				float m_unitTimeSpentOnA;
			};
		}
	}
}