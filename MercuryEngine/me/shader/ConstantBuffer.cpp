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
	case ConstantsType::Float3: return "Float3";
	case ConstantsType::Float4: return "Float4";
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
	else if ( unify::StringIs( "Float1", value ) )
	{
		return ConstantsType::Float1;
	}
	else if ( unify::StringIs( "Float2", value ) )
	{
		return ConstantsType::Float2;
	}
	else if ( unify::StringIs( "Float3", value ) )
	{
		return ConstantsType::Float3;
	}
	else if ( unify::StringIs( "Float4", value ) )
	{
		return ConstantsType::Float4;
	}

	throw std::exception( std::string( std::string( "Unknown shader constants type string, \"" ) + value + "\"" ).c_str() );
}

size_t ConstantsType::SizeOf(TYPE value)
{
	switch (value)
	{
	case ConstantsType::Matrix: return sizeof( unify::Matrix );
	case ConstantsType::Float1: return sizeof( float ) * 1;
	case ConstantsType::Float2: return sizeof( float ) * 2;
	case ConstantsType::Float3: return sizeof( float ) * 3;
	case ConstantsType::Float4: return sizeof( float ) * 4;
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

		if ( child.GetType() == qxml::Element::NodeType::Element )
		{
			ConstantsType::TYPE type = ConstantsType::FromString( child.GetTagName() );
			std::string name = child.GetAttributeElse< std::string >( "name", "PADDING" );
			size_t count = child.GetAttributeElse< size_t >( "count", 1 );

			ConstantVariable variable( name, type, count );
			
			auto && default = child.GetElement( "default" );
			if ( default != nullptr )
			{
				switch ( type )
				{
				case ConstantsType::Matrix:
				case ConstantsType::Float1:
				case ConstantsType::Float2:
				case ConstantsType::Float3:
				case ConstantsType::Float4:
					{
						std::string text = default->GetText();
						size_t left = 0;
						size_t right = 0;
						do
						{
							right = text.find( ',', left );
							std::string value = text.substr( left, right - left );
							variable.default.push_back( unify::Cast< float >( value ) );
							left = right + 1;
						} while ( right != std::string::npos );
						break;
					}
				}
				variable.hasDefault = true;
			}
			else
			{
				variable.hasDefault = false;
			}

			AddVariable( variable );
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
