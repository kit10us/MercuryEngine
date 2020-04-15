// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaColor.h>
#include <me/dyna/IDynaDelta.h>

namespace me
{
	namespace dyna
	{
		namespace color
		{
			/// <summary>
			/// Applies a IDynaDelta to a IDynaColor
			/// </summary>
			class Delta : public IDynaColor
			{
			public:
				Delta( IDynaColor::ptr rotaiton, IDynaDelta::ptr delta );

				Result GetColor(unify::ColorUnit & out, unify::TimeDelta delta) override;

			private:
				IDynaColor::ptr m_color;
				IDynaDelta::ptr m_delta;
			};
		}
	}
}