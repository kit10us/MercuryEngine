// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/motivator/IInputMotivator.h>
#include <map>

namespace me
{
	namespace motivator
	{
		///<summary>
		/// Contains motivators.
		///</summary>
		class InputMotivator : public IInputMotivator
		{
		public:
			InputMotivator();
			~InputMotivator();

			void AddMotivation( std::string motiviation, input::IInputCondition::ptr condition ) override;
			size_t GetMotivationCount() const override;
			std::string GetMotivationName( size_t index ) const override;
			input::IInputCondition::ptr GetCondition( size_t index ) const override;
			input::IInputCondition::ptr GetCondition( std::string name ) const override;
			
		private:
			std::map< std::string /*motivation*/, size_t /*index*/, unify::CaseInsensitiveLessThanTest > m_motivationsMap;
			std::vector< input::IInputCondition::ptr > m_motivationConditions;
			std::vector< std::string > m_motivationNames;
		};
	}
}