// Copyright (c) 2002 - 2018, Quentin S. Smith
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

				Result GetPosition(unify::V3< float > & out, float delta) override;

			private:
				IDynaPosition::ptr m_positionA;
				IDynaPosition::ptr m_positionB;
			};
		}
	}
}