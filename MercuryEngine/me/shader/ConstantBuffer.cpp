// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/shader/ConstantBuffer.h>
#include <unify/String.h>
#include <unify/Matrix.h>

using namespace me;
using namespace shader;

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
	
	for ( auto && child : node->Children( "element" ) )
	{	
		ElementFormat::TYPE type = ElementFormat::FromString( child.GetAttribute< std::string >( "type" ) );
		std::string name = child.GetAttributeElse< std::string >( "name", "PADDING" );
		size_t count = child.GetAttributeElse< size_t >( "count", 1 );

		ConstantVariable variable( name, type, count );
			
		auto && default = child.GetElement( "default" );
		if ( default != nullptr )
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
			variable.hasDefault = true;
		}
		else
		{
			variable.hasDefault = false;
		}

		AddVariable( variable );
	}
}

size_t ConstantBuffer::AddVariable( ConstantVariable variable )
{
	size_t sizeInBytes = ElementFormat::SizeOf( variable.type );
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
