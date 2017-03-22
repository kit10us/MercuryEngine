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
			/// Adds a scale to another position.
			/// </summary>
			class Add : public IDynaDelta
			{
			public:
				Add( IDynaDelta::ptr deltaA );

				Result GetDelta(float & delta) override;

			private:
				IDynaDelta::ptr m_deltaA;
			};
		}
	}
}