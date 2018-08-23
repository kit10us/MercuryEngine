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
			/// Adds a scale to another position.
			/// </summary>
			class Add : public IDynaDelta
			{
			public:
				Add( IDynaDelta::ptr deltaA );

				Result GetDelta( unify::TimeDelta & delta) override;

			private:
				IDynaDelta::ptr m_deltaA;
			};
		}
	}
}