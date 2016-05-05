// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>
#include <memory>

namespace qxml
{
	class Attribute
	{
	public:
		typedef std::shared_ptr< Attribute > shared_ptr;

		Attribute();
		explicit Attribute( const std::string & attribute );
		Attribute( const std::string & name, const std::string & value );
		~Attribute();
		const std::string & GetName() const; 
		float GetFloat() const;
		const std::string & GetString() const;
		int GetInteger() const;
		bool GetBoolean() const;

		template< typename T >
		T Get() const
		{
			return unify::Cast< T >( m_value );
		}

	protected:
		std::string m_name;
		std::string m_value;
	};

	template< > bool Attribute::Get< bool >() const;
} // namespace qxml