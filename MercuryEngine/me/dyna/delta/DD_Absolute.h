// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaDelta.h>

namespace me
{
	namespace dyna
	{
		namespace delta
		{
			/// <summary>
			/// A IDynaDelta that's an absolute value.
			/// </summary>
			class Absolute : public IDynaDelta
			{
			public:
				Absolute( float delta );

				Result GetDelta( float & delta ) override;

			private:
				float m_delta;
			};
		}
	}
}