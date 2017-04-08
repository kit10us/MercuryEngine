// Copyright (c) 2002 - 2018, Quentin S. Smith
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
			/// Mulliplies a delta against a IDynaDeta.
			/// </summary>
			class Mul : public IDynaDelta
			{
			public:
				Mul( IDynaDelta::ptr deltaA );

				Result GetDelta( float & delta ) override;

			private:
				IDynaDelta::ptr m_deltaA;
			};
		}
	}
}