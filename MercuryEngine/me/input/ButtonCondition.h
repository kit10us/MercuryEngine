// Copyright (c) 2002 - 2018, Evil Quail LLC
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
			ButtonCondition( size_t subSource, std::string name, bool down );
			virtual ~ButtonCondition();

			bool IsTrue( IInputDevice* device ) const override;
			unify::V3< float > GetValue( IInputDevice* device ) const override;

		private:
			bool m_down;
		};
	}
}