// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/InputCondition.h>
#include <unify/Range.h>

namespace me
{
	namespace input
	{
		class StickCondition : public InputCondition
		{
		public:
			StickCondition( size_t subSource, std::string name, unify::V3< unify::Range< float > > low, unify::V3< unify::Range< float > > high );
			virtual ~StickCondition();

			bool IsTrue( IInputDevice* device ) const override;
			unify::V3< float > GetValue( IInputDevice* device ) const override;

		private:
			const unify::V3< unify::Range< float > > m_low;
			const unify::V3< unify::Range< float > > m_high;
		};
	}
}