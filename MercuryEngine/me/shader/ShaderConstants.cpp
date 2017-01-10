// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/shader/ShaderConstants.h>
#include <me/exception/FailedToCreate.h>
#include <unify/String.h>
#include <unify/Matrix.h>

using namespace me;
using namespace shader;

ShaderConstants::ShaderConstants()
	: m_hasDefaults{ 0 }
{
}

ShaderConstants::ShaderConstants( const qxml::Element * node )
	: ShaderConstants()
{
	if ( ! node ) throw exception::FailedToCreate( "Shader constants XML node is null!" );

	for ( auto && child : node->Children() )
	{
		if ( !child.IsTagName( "buffer" ) ) continue;

		ConstantBuffer::ptr buffer( new ConstantBuffer( &child ) );
		AddBuffer( buffer );
	}
}

void ShaderConstants::AddBuffer( ConstantBuffer::ptr buffer )
{
	m_buffers.push_back( buffer );

	size_t i = 0;
	for ( auto && variable : buffer->GetVariables() )
	{
		size_t bufferIndex = m_buffers.size() - 1;
		m_map[variable.name].buffer = bufferIndex;
		m_map[variable.name].index = i;

		if ( unify::StringIs( variable.name, "world" ) || unify::StringIs( variable.name, "worldmatrix" ) )
		{
			if ( ! m_world.IsSet() )
			{
				m_world.buffer = bufferIndex;
				m_world.index = i;
				m_world.offsetInBytes = variable.offsetInBytes;
			}
		}
		else if ( unify::StringIs( variable.name, "view" ) || unify::StringIs( variable.name, "viewmatrix" ) )
		{
			if ( ! m_view.IsSet() )
			{
				m_view.buffer = bufferIndex;
				m_view.index = i;
				m_view.offsetInBytes = variable.offsetInBytes;
			}
		}
		else if ( unify::StringIs( variable.name, "projection" ) || unify::StringIs( variable.name, "projectionmatrix" ) )
		{
			if ( ! m_projection.IsSet() )
			{
				m_projection.buffer = bufferIndex;
				m_projection.index = i;
				m_projection.offsetInBytes = variable.offsetInBytes;
			}
		}

		if ( variable.hasDefault )
		{
			m_hasDefaults |= 1 << bufferIndex;
		}

		i++;
	}
}

const std::vector< ConstantBuffer::ptr > & ShaderConstants::GetBuffers() const
{
	return m_buffers;
}

Reference ShaderConstants::FindDefinition( std::string name ) const
{
	auto itr = m_map.find( name );
	if ( itr == m_map.end() ) return Reference();
	return itr->second;
}

Reference ShaderConstants::GetWorld() const
{
	return m_world;
}

Reference ShaderConstants::GetView() const
{
	return m_view;
}

Reference ShaderConstants::GetProjection() const
{
	return m_projection;
}

bool ShaderConstants::HasDefaults( size_t buffer ) const
{
	return (1 << buffer) == (m_hasDefaults & (1 << buffer));
}

