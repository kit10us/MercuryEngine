// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <me/Lookup.h>

using namespace me;

Lookup::Lookup()
{
}

void Lookup::AddValue( std::string name, std::string value )
{	
	auto itr = m_valueMap.find( name );
	if ( itr != m_valueMap.end() )
	{
		m_values[ itr->second ] = value;
	}
	else
	{
		m_valueMap[ name ] = m_values.size();
		m_values.push_back( value );
	}
}

bool Lookup::Exists( std::string name ) const
{
	auto itr = m_valueMap.find( name );
	if ( itr == m_valueMap.end() )
	{
		return false;
	}
	else
	{
		return true;
	}
}

size_t Lookup::Find( std::string name ) const
{
	auto itr = m_valueMap.find( name );
	if ( itr == m_valueMap.end() )
	{
		throw exception::OutOfBounds( "Lookup did not contain a values called \"" + name + "\"!" );
	}
	else
	{
		return itr->second;
	}
}

bool Lookup::SetValue( std::string name, std::string value )
{
	auto itr = m_valueMap.find( name );
	if ( itr == m_valueMap.end() )
	{
		m_valueMap[ name ] = m_values.size();
		m_values.push_back( value );
		m_names.push_back( name );
	}
	else
	{
		m_values[ itr->second ] = value;
	}

	return true;
}

std::string Lookup::GetValue( std::string name ) const
{
	auto itr = m_valueMap.find( name );
	if ( itr == m_valueMap.end() )
	{
		return "";
	}
	else
	{
		return m_values[ itr->second ];
	}
}

bool Lookup::SetValue( size_t index, std::string value )
{
	if ( index >= m_values.size() )
	{
		return false;
	}

	m_values[ index ] = value;
	return true;
}

std::string Lookup::GetValue( size_t index ) const
{
	if ( index >= m_values.size() )
	{
		return "";
	}

	return m_values[ index ];
}

size_t Lookup::Count() const
{
	return m_values.size();
}

std::string Lookup::GetName( size_t index ) const
{
	if ( index >= m_values.size() )
	{
		return "";
	}

	return m_names[ index ];
}
