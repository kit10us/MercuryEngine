// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/pi/PhysicsEnvironment.h>

namespace dxi
{
	namespace pi
	{
		class EnvironmentConstant : public PhysicsEnvironment
		{
		public:
			EnvironmentConstant();
			EnvironmentConstant( const unify::V3< float > & force );
			~EnvironmentConstant();

			void SetForce( const unify::V3< float > & force );
			const unify::V3< float > & GetForce() const;
			void ApplyEffect( float delta, PhysicsInstance * object );
		private:
			unify::V3< float > m_force;
		};
	} // namespace pi
} // namespace dxi