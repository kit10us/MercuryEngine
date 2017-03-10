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
			ButtonPressedCondition( IInputSource::ptr source, size_t subSource, std::string name );
			virtual ~ButtonPressedCondition();

			bool IsTrue() const override;
			float GetValue() const override;
		};
	}
}