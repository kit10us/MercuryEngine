// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaRotation.h>
#include <me/dyna/IDynaDelta.h>

namespace me
{
	namespace dyna
	{
		namespace rotation
		{
			/// <summary>
			/// Applies a IDynaDelta to a IDynaRotation
			/// </summary>
			class Delta : public IDynaRotation
			{
			public:
				Delta( IDynaRotation::ptr rotaiton, IDynaDelta::ptr delta );

				Result GetRotation(unify::Quaternion & out, unify::TimeDelta delta) override;

			private:
				IDynaRotation::ptr m_rotation;
				IDynaDelta::ptr m_delta;
			};
		}
	}
}