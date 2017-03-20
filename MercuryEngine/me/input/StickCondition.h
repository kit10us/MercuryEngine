// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/InputCondition.h>

namespace me
{
	namespace input
	{
		enum class StickAxis
		{
			X, Y, Z
		};
	 	StickAxis StickAxisFromString( std::string axis );

		class StickCondition : public InputCondition
		{
		public:
			StickCondition( IInputSource::ptr source, size_t subSource, std::string name, StickAxis axis, float cap_low, float threshold_low, float threshold_high, float cap_high );
			virtual ~StickCondition();

			bool IsTrue() const override;
			float GetValue() const override;

		private:
			StickAxis m_axis;
			float m_cap_low;
			float m_threshold_low;
			float m_threshold_high;
			float m_cap_high;
		};
	}
}