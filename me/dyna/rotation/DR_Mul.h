// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaRotation.h>

namespace me
{
	namespace dyna
	{
		namespace rotation
		{
			/// <summary>
			/// Multiplies a scale to another rotation.
			/// </summary>
			class Mul : public IDynaRotation
			{
			public:
				Mul( IDynaRotation::ptr rotationA, IDynaRotation ::ptr rotationB );

				Result GetRotation(unify::Quaternion & out, unify::TimeDelta delta) override;

			private:
				IDynaRotation::ptr m_rotationA;
				IDynaRotation::ptr m_rotationB;
			};
		}
	}
}