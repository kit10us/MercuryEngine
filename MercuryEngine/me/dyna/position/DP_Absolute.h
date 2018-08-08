// Copyright (c) 2002 - 2018, Evil Quail LLC
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
			/// An absolute position.
			/// </summary>
			class Absolute : public IDynaPosition
			{
			public:
				Absolute(unify::V3< float > v3);

				Result GetPosition(unify::V3< float > & out, float delta) override;

			private:
				unify::V3< float > m_v3;
			};
		}
	}
}