// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaPosition.h>
#include <me/object/Object.h>

namespace me
{
	namespace dyna
	{
		namespace position
		{
			/// <summary>
			/// An Object's position.
			/// </summary>
			class Object : public IDynaPosition
			{
			public:
				Object( object::Object * object );

				Result GetPosition(unify::V3< float > & out, unify::TimeDelta delta) override;

			private:
				object::Object * m_object;
			};
		}
	}
}