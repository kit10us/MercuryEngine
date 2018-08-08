// Copyright (c) 2002 - 2018, Evil Quail LLC
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
			/// An absolute rotation.
			/// </summary>
			class Absolute : public IDynaRotation
			{
			public:
				Absolute(unify::Quaternion q);

				Result GetRotation(unify::Quaternion & out, float delta) override;

			private:
				unify::Quaternion m_q;
			};
		}
	}
}