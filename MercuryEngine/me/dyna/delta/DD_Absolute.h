// Copyright (c) 2002 - 2018, Evil Quail LLC
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
				Absolute( unify::TimeDelta delta );

				Result GetDelta( unify::TimeDelta & delta ) override;

			private:
				unify::TimeDelta m_delta;
			};
		}
	}
}