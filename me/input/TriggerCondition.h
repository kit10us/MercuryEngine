// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/input/InputCondition.h>
#include <me/input/IInputDevice.h>

namespace me
{
	namespace input
	{
		class TriggerCondition : public InputCondition
		{
		public:
			TriggerCondition( size_t subSource, std::string name, float threshold, float cap );
			virtual ~TriggerCondition();

			bool IsTrue( IInputDevice* device ) const override;

			unify::V3< float > GetValue( IInputDevice* device ) const override;

		private:
			float m_threshold;
			float m_cap;
		};
	}
}