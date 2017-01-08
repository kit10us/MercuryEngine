// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/shader/ConstantBuffer.h>
#include <unify/String.h>
#include <unify/Matrix.h>

using namespace me;
using namespace shader;

std::string ConstantsType::ToString(TYPE value)
{
	switch (value)
	{
	case ConstantsType::Matrix: return "Matrix";
	default:
		throw std::exception( "Unknown shader constants type value!" );
	}
}

ConstantsType::TYPE ConstantsType::FromString(std::string value)
{
	if ( unify::StringIs( "Matrix", value ) )
	{
		return ConstantsType::Matrix;
	}

	throw std::exception( std::string( std::string( "Unknown shader constants type string, \"" ) + value + "\"" ).c_str() );
}

size_t ConstantsType::SizeOf(TYPE value)
{
	switch (value)
	{
	case ConstantsType::Matrix: return sizeof unify::Matrix;
	default:
		throw std::exception( "Unknown shader constants type value!" );
	}
}

ConstantBuffer::ConstantBuffer( std::string name )
	: m_name{ name }
	, m_sizeInBytes{ 0 }
{
}


ConstantBuffer::ConstantBuffer( const qxml::Element * node )
	: m_sizeInBytes{ 0 }
{
	if ( node->HasAttributes( "name" ) )
	{
		m_name = node->GetAttribute< std::string >( "name" );
	}			
	
	for ( auto && child : node->Children() )
	{	
		if ( child.GetType() != qxml::Element::NodeType::Element ) continue;

		if ( child.IsTagName( "matrix" ) )
		{
			ConstantsType::TYPE type = ConstantsType::FromString( child.GetTagName() );
			std::string name = child.GetAttribute< std::string >( "name" );
			size_t count = child.GetAttributeElse< size_t >( "count", 1 );
			AddVariable( { name, type, count } );
		}
	}
}

size_t ConstantBuffer::AddVariable( ConstantVariable variable )
{
	size_t sizeInBytes = ConstantsType::SizeOf( variable.type );
	variable.offsetInBytes = m_sizeInBytes;
	m_sizeInBytes += sizeInBytes * variable.count;
	m_variables.push_back( variable );
	return m_variables.size() - 1;
}

std::string ConstantBuffer::GetName() const
{
	return m_name;
}

size_t ConstantBuffer::GetSizeInBytes() const
{
	return m_sizeInBytes;
}

const std::vector< ConstantVariable > & ConstantBuffer::GetVariables() const
{
	return m_variables;
}
