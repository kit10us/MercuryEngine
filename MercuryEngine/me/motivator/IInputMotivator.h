// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputSource.h>
#include <me/input/IInputCondition.h>

namespace me
{
	namespace motivator
	{
		///<summary>
		/// Contains motivators.
		///</summary>
		class IInputMotivator
		{
		public:
			virtual ~IInputMotivator() {}

			virtual void AddMotivation( std::string motiviation, input::IInputCondition::ptr condition ) = 0;
			virtual size_t GetMotivationCount() const = 0;
			virtual std::string GetMotivationName( size_t index ) const = 0;
			virtual input::IInputCondition::ptr GetCondition( size_t index ) const = 0;
			virtual input::IInputCondition::ptr GetCondition( std::string name ) const = 0;
		};
	}
}