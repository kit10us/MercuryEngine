// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <qxml/Attribute.h>
#include <unify/String.h>

using namespace qxml;

Attribute::Attribute()
{
}

Attribute::Attribute( const std::string & attribute )
{
	m_name = unify::ListPart( attribute, '=', 0 );
	m_value = unify::ListPart( attribute, '=', 1 );
	
	if( unify::LeftString( m_value, 1 ) == "\"" )
	{
		m_value = m_value.substr( 1, m_value.length() - 1 );
	}

	if( unify::RightString( m_value, 1 ) == "\"" )
	{
		m_value = m_value.substr( 0, m_value.length() - 1 );
	}
}

Attribute::Attribute( const std::string & name, const std::string & value )
: m_name( name )
, m_value( value )
{
}

Attribute::~Attribute()
{
	m_name.clear();
	m_value.clear();
}

const std::string & Attribute::GetName() const
{
	return m_name;
}

float Attribute::GetFloat() const
{
	if( m_value.empty() )
	{
		return 0.0f;
	}

	return unify::Cast< float >( m_value );
}

const std::string & Attribute::GetString() const
{
	return m_value;
}

int Attribute::GetInteger() const
{
	if( m_value.empty() ) return 0;
	return unify::Cast< int >( m_value );
}

bool Attribute::GetBoolean() const
{
	if( m_value.empty() ) return 0;
	return ! ( unify::StringIs( m_value, "false" ) || m_value.compare( "0" ) == 0 );
}
