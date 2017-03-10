// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/InputCondition.h>

namespace me
{
	namespace input
	{
		class TriggerCondition : public InputCondition
		{
		public:
			TriggerCondition( IInputSource::ptr source, size_t subSource, std::string name, float threshold, float cap );
			virtual ~TriggerCondition();

			bool IsTrue() const override;
			float GetValue() const override;

		private:
			float m_threshold;
			float m_cap;
		};
	}
}