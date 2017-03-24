// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputCondition.h>
#include <me/input/IInputDevice.h>

namespace me
{
	namespace input
	{
		class InputCondition : public IInputCondition
		{
		public:
			InputCondition( size_t subSource, std::string name );
			virtual ~InputCondition();

			std::string GetName() const;
			size_t GetSubSource() const;

		private:
			size_t m_subSource;
			std::string m_name;
		};
	}
}