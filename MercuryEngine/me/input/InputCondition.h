// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputCondition.h>
#include <me/input/IInputSource.h>

namespace me
{
	namespace input
	{
		class InputCondition : public IInputCondition
		{
		public:
			InputCondition( IInputSource::ptr source, size_t subSource, size_t index );
			virtual ~InputCondition();

			IInputSource::ptr GetSource() const override;
			size_t GetIndex() const;
			size_t GetSubSource() const;

		private:
			IInputSource::ptr m_source;
			size_t m_index;
			size_t m_subSource;
		};
	}
}