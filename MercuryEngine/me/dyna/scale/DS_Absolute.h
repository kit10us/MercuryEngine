// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaScale.h>

namespace me
{
	namespace dyna
	{
		namespace scale
		{
			/// <summary>
			/// An absolute scale.
			/// </summary>
			class Absolute : public IDynaScale
			{
			public:
				Absolute(unify::V3< float > v3);

				Result GetScale(unify::V3< float > & out, float delta) override;

			private:
				unify::V3< float > m_v3;
			};
		}
	}
}