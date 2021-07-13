// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/ConstantTable.h>
#include <unify/String.h>
#include <unify/Matrix.h>

using namespace me;
using namespace render;

ConstantTable::ConstantTable()
	: m_hasDefaults{ 0 }
{
}				 

ConstantTable::ConstantTable( const qxml::Element * node )
	: m_hasDefaults{ 0 }
{
	for ( auto && bufferNode : node->Children( "buffer" ) )
	{	
		AddBuffer( bufferNode.GetAttributeElse< std::string >( "name", "" ) );
		for ( auto && elementNode : bufferNode.Children( "element" ) )
		{
			ElementFormat::TYPE type = ElementFormat::FromString( elementNode.GetAttribute< std::string >( "type" ) );
			std::string name = elementNode.GetAttributeElse< std::string >( "name", "PADDING" );
			size_t count = elementNode.GetAttributeElse< size_t >( "count", 1 );

			ConstantVariable variable( name, type, count );

			auto && default_value = elementNode.GetElement( "default" );
			if (default_value != nullptr )
			{
				std::string text = default_value->GetText();
				size_t left = 0;
				size_t right = 0;
				do
				{
					right = text.find( ',', left );
					std::string value = text.substr( left, right - left );
					variable.defaultValue.push_back( unify::Cast< float >( value ) );
					left = right + 1;
				} while ( right != std::string::npos );
				variable.hasDefault = true;
			}
			else
			{
				variable.hasDefault = false;
			}

			AddVariable(  m_sizeInBytes.size() - 1, variable );
		}
	}
}

void ConstantTable::AddBuffer( std::string name )
{
	m_variables.push_back( std::vector< ConstantVariable >() );
	m_name.push_back( name );
	m_sizeInBytes.push_back( 0 );
}

size_t ConstantTable::BufferCount() const
{
	return m_sizeInBytes.size();
}

size_t ConstantTable::AddVariable( size_t bufferIndex, ConstantVariable variable )
{
	assert( bufferIndex < m_sizeInBytes.size() );

	size_t myIndex = m_variables[bufferIndex].size();

	size_t sizeInBytes = ElementFormat::SizeOf( variable.type );
	variable.offsetInBytes = m_sizeInBytes[ bufferIndex ];
	m_sizeInBytes[ bufferIndex ] += sizeInBytes * variable.count;
	m_variables[ bufferIndex ].push_back( variable );

	Reference myReference( bufferIndex, myIndex, variable.offsetInBytes );

	m_map[variable.name].buffer = bufferIndex;
	m_map[variable.name].index = myIndex;

	if ( unify::string::StringIs( variable.name, "world" ) || unify::string::StringIs( variable.name, "worldmatrix" ) )
	{
		if ( !m_world.IsSet() )
		{
			m_world = myReference;
		}
	}
	else if ( unify::string::StringIs( variable.name, "view" ) || unify::string::StringIs( variable.name, "viewmatrix" ) )
	{
		if ( !m_view.IsSet() )
		{
			m_view = myReference;
		}
	}
	else if ( unify::string::StringIs( variable.name, "projection" ) || unify::string::StringIs( variable.name, "projectionmatrix" ) )
	{
		if ( !m_projection.IsSet() )
		{
			m_projection = myReference;
		}
	}

	if ( variable.hasDefault )
	{
		m_hasDefaults |= 1 << bufferIndex;
	}			  

	return myIndex;
}

size_t ConstantTable::GetSizeInBytes( size_t bufferIndex ) const
{
	assert( bufferIndex < m_sizeInBytes.size() );

	return m_sizeInBytes[ bufferIndex ];
}

const std::vector< ConstantVariable > & ConstantTable::GetVariables( size_t bufferIndex ) const
{
	assert( bufferIndex < m_sizeInBytes.size() );

	return m_variables[ bufferIndex ];
}

const ConstantVariable & ConstantTable::GetVariable( const Reference & reference ) const
{
	return m_variables[reference.buffer][reference.index];
}

Reference ConstantTable::FindDefinition( std::string name ) const
{
	auto itr = m_map.find( name );
	if ( itr == m_map.end() ) return Reference();
	return itr->second;
}

Reference ConstantTable::GetWorld() const
{
	return m_world;
}

Reference ConstantTable::GetView() const
{
	return m_view;
}

Reference ConstantTable::GetProjection() const
{
	return m_projection;
}

bool ConstantTable::HasDefaults( size_t buffer ) const
{
	return (1 << buffer) == (m_hasDefaults & (1 << buffer));
}
