// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/pi/PhysicsEnvironment.h>
#include <unify/BBox.h>

namespace dxi
{
	namespace pi
	{
		// A physics environment limited to a boxed area.
		class EnvironmentBox : public PhysicsEnvironment
		{
		public:
			EnvironmentBox();
			EnvironmentBox( const unify::Frame & frame, const unify::BBox< float > & bbox, const unify::V3< float > & force );
			~EnvironmentBox();

			void SetFrame( const unify::Frame & frame );
			const unify::Frame & GetFrame() const;
			void SetBBox( const unify::BBox< float > & bbox );
			const unify::BBox< float > & GetBBox() const;
			void SetForce( const unify::V3< float > & force );
			const unify::V3< float > & GetForce() const;
			void ApplyEffect( float delta, PhysicsInstance * object );
		private:
			unify::Frame m_frame;
			unify::BBox< float > m_bbox;
			unify::V3< float > m_force;
		};
	} // namespace pi
} // namespace dxi