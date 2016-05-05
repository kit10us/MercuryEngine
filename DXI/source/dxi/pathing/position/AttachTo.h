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
			class AttachTo : public Modifier
			{
			public:
				AttachTo( const unify::Frame * targetFrame, float distance, const unify::Quaternion & axisFromTarget );
				AttachTo( const unify::V3< float > * targetPosition, float distance, const unify::Quaternion & axisFromTarget );
				void AtTime( float unitTime, unify::Frame * frame );
			private:
				const unify::Frame * m_targetFrame;
				const unify::V3< float > * m_targetPosition;
				unify::Quaternion m_axisFromTarget;
				float m_distance;
			};
		}
	}
}