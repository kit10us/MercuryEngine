// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputSource.h>

namespace me
{
	namespace input
	{
		class IInputCondition
		{
		public:
			typedef std::shared_ptr< IInputCondition > ptr;

			virtual ~IInputCondition() {}
			virtual bool operator()() const = 0;
		};

		class InputCondition : public IInputCondition
		{
		public:
			InputCondition( IInputSource::ptr source, size_t subSource, size_t index );
			virtual ~InputCondition();

			IInputSource::ptr GetSource() const;
			size_t GetIndex() const;
			size_t GetSubSource() const;

		private:
			IInputSource::ptr m_source;
			size_t m_index;
			size_t m_subSource;
		};

		class ButtonCondition : public InputCondition
		{
		public:
			ButtonCondition( IInputSource::ptr source, size_t subSource, std::string name, std::string condition );
			virtual ~ButtonCondition();

			size_t GetCondition() const;

			bool operator()() const override;

		private:
			size_t m_condition;
		};

		IInputCondition::ptr MakeButtonCondition( IInputSource::ptr source, size_t subSource, std::string name, std::string condition );
	}
}