// Copyright (c) 2002 - 2018, Evil Quail LLC
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

			std::string GetName() const override;
			size_t GetSubSource() const;

		private:
			size_t m_subSource;
			std::string m_name;
		};
	}
}