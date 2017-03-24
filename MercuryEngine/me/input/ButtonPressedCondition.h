// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/InputCondition.h>

namespace me
{
	namespace input
	{
		class ButtonPressedCondition : public InputCondition
		{
		public:
			ButtonPressedCondition( size_t subSource, std::string name );
			virtual ~ButtonPressedCondition();

			bool IsTrue( IInputDevice* device ) const override;
			unify::V3< float > GetValue( IInputDevice* device ) const override;
		};
	}
}