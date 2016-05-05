// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/pathing/Modifier.h>	

namespace dxi
{
	namespace pathing
	{
		namespace position
		{
			class RotateAbout : public Modifier
			{
			public:
				RotateAbout( const unify::V3< float > * position, float distance, const unify::Quaternion * axisFrom, const unify::Quaternion * axisTo );
				void AtTime( float unitTime, unify::Frame * frame );
			private:
				const unify::V3< float > * m_position;
				const unify::Quaternion * m_axisFrom;
				const unify::Quaternion * m_axisTo;
				float m_distance;
			};
		}
	}
}