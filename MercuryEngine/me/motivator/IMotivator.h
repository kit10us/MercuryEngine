														 // Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

namespace me
{
	/// <summary>
	/// A motivator allows controllers to interact with objects. Controllers can be players, or paths, or any other
	/// source of input/motivation/force.
	/// </summary>
	class IMotivator
	{
	public:
		virtual ~IMotivator() {}

		virtual int GetValueCount() const = 0;
		virtual bool ValueExists( std::string name ) const = 0;
		virtual std::string GetValueName( size_t index ) const = 0;
		virtual int FindValueIndex( std::string name ) const = 0;
		virtual void SetValue( size_t index, std::string value ) = 0;
		virtual void SetValue( std::string name, std::string value ) = 0;
		virtual std::string GetValue( size_t index ) const = 0;
		virtual std::string GetValue( std::string name ) const = 0;
	};
}