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
			virtual bool IsTrue() const = 0;
			virtual float GetValue() const = 0;
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
			ButtonCondition( IInputSource::ptr source, size_t subSource, std::string name, bool down );
			virtual ~ButtonCondition();

			bool IsTrue() const override;
			float GetValue() const override;

		private:
			bool m_down;
		};

		class ButtonPressedCondition : public InputCondition
		{
		public:
			ButtonPressedCondition( IInputSource::ptr source, size_t subSource, std::string name );
			virtual ~ButtonPressedCondition();

			bool IsTrue() const override;
			float GetValue() const override;
		};
		

		enum class StickAxis
		{
			X, Y, Z
		};
	 	StickAxis StickAxisFromString( std::string axis );

		class StickCondition : public InputCondition
		{
		public:
			StickCondition( IInputSource::ptr source, size_t subSource, std::string name, StickAxis axis, float cap_low, float threshhold_low, float threshhold_high, float cap_high );
			virtual ~StickCondition();

			bool IsTrue() const override;
			float GetValue() const override;

		private:
			StickAxis m_axis;
			float m_cap_low;
			float m_threshhold_low;
			float m_threshhold_high;
			float m_cap_high;
		};

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