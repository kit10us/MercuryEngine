// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/InputCondition.h>

namespace me
{
	namespace input
	{
		class ButtonCondition : public InputCondition
		{
		public:
			ButtonCondition( IInputSource::ptr source, size_t subSource, std::string name, bool down );
			virtual ~ButtonCondition();

			bool IsTrue() const override;
			float GetValue() const override;

		private:
			bool m_down;
		};
	}
}