// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaPosition.h>

namespace me
{
	namespace dyna
	{
		namespace position
		{
			/// <summary>
			/// Muls a position to another position.
			/// </summary>
			class Mul : public IDynaPosition
			{
			public:
				Mul( IDynaPosition::ptr positionA, IDynaPosition::ptr positionB );

				Result GetPosition(unify::V3< float > & out, unify::TimeDelta delta) override;

			private:
				IDynaPosition::ptr m_positionA;
				IDynaPosition::ptr m_positionB;
			};
		}
	}
}