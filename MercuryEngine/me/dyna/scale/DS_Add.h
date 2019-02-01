// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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
			/// Adds a scale to another position.
			/// </summary>
			class Add : public IDynaScale
			{
			public:
				Add( IDynaScale::ptr scaleA, IDynaScale ::ptr scaleB );

				Result GetScale(unify::V3< float > & out, unify::TimeDelta delta) override;

			private:
				IDynaScale::ptr m_scaleA;
				IDynaScale::ptr m_scaleB;
			};
		}
	}
}