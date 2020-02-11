// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaPosition.h>
#include <me/dyna/IDynaDelta.h>

namespace me
{
	namespace dyna
	{
		namespace position
		{
			/// <summary>
			/// Applies a IDynaDelta to a IDynaPosition
			/// </summary>
			class Delta : public IDynaPosition
			{
			public:
				Delta( IDynaPosition::ptr rotaiton, IDynaDelta::ptr delta );

				Result GetPosition(unify::V3< float > & out, unify::TimeDelta delta) override;

			private:
				IDynaPosition::ptr m_position;
				IDynaDelta::ptr m_delta;
			};
		}
	}
}