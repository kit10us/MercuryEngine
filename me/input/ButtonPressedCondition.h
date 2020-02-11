// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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