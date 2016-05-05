// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/pathing/Modifier.h>	

namespace dxi
{
	namespace pathing
	{
		namespace orientation
		{
			// Affects orientation only (based on position)
			class LookAt : public Modifier
			{
			public:
				LookAt( const unify::V3< float > * position );
				LookAt( const unify::Frame * frame );
				void AtTime( float unitTime, unify::Frame * frame );

			private:
				const unify::V3< float > * m_position;
				const unify::Frame * m_frame;
			};
		}
	}
}