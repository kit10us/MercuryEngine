// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/render/VertexDeclaration.h>
#include <me/render/IRenderer.h>
#include <me/render/IVertexShader.h>
#include <unify/ColorUnit.h>
#include <algorithm>

using namespace me;
using namespace render;
									  
VertexDeclaration::VertexDeclaration()
	: m_instancing( Instancing::None )
	, m_numberOfSlots( 0 )
	, m_instanceingSlot{ -1 }
{
}

VertexDeclaration::VertexDeclaration( const qxml::Element * xml )
	: VertexDeclaration()
{
	for( auto && slot : xml->Children( "slot" ) )
	{
		m_numberOfSlots++;
		m_sizeInBytes.push_back( 0 );
		m_elements.push_back( std::vector< VertexElement >() );
		m_instancing.push_back( Instancing::FromString( slot.GetAttributeElse< std::string >( "instancing", "none" ) ) );
		m_instances.push_back( slot.GetAttributeElse( "instances", 0 ) );
		if ( m_instancing.back() != Instancing::None )
		{
			m_instanceingSlot = m_numberOfSlots - 1;
		}
		
		SlotClass::TYPE	slotClass = SlotClass::FromString( slot.GetAttributeElse< std::string >( "class", "vertex" ) );
		size_t instanceDataStepRate = slot.GetAttributeElse( "instancedatasteprate", 0 );

		for ( auto && element : slot.Children( "element" ) )
		{
			VertexElement vertexElement( element );
			vertexElement.InputSlot = m_numberOfSlots - 1;
			vertexElement.AlignedByteOffset = m_sizeInBytes[ vertexElement.InputSlot ];
			vertexElement.SlotClass = slotClass;
			vertexElement.InstanceDataStepRate = instanceDataStepRate;

			std::string name = vertexElement.SemanticName;	  
			m_elementMap[name] = { m_numberOfSlots - 1, m_allElements.size() }; // The next index is size, so this is what we will reference from our map.
			m_allElements.push_back( vertexElement );
			m_elements[vertexElement.InputSlot].push_back( vertexElement );

			m_sizeInBytes[ vertexElement.InputSlot ] += vertexElement.SizeOf();
		}
	}
}

VertexDeclaration::VertexDeclaration( const qjson::Object json )
	: VertexDeclaration()
{
	for ( auto itr : json )
	{
		const qjson::Object * member = dynamic_cast< const qjson::Object * >(itr.GetValue().get());
		if ( member != nullptr )
		{
			assert( 0 ); //TODO: What?	
		}
		else // Simple string as type.
		{
			VertexElement element( itr );
	
			std::string name = itr.GetName();

			if ( element.InputSlot >= m_numberOfSlots )
			{
				m_numberOfSlots = element.InputSlot + 1; // Ensure we track all slots.
				m_sizeInBytes.resize( m_numberOfSlots, 0 );
				m_elements.resize( m_numberOfSlots, std::vector< VertexElement >() );
			}
	
			element.AlignedByteOffset = m_sizeInBytes[ element.InputSlot ];
			
			m_elementMap[name] = { element.InputSlot, m_allElements.size() }; // The next index is size, so this is what we will reference from our map.
			m_allElements.push_back( element );
			m_elements[ element.InputSlot ].push_back( element );
	
			m_sizeInBytes[ element.InputSlot ] += element.SizeOf();
		}
	}	
}

VertexDeclaration::~VertexDeclaration()
{
		Destroy();
}

void VertexDeclaration::Build( IRenderer * renderer, const IVertexShader & vs )
{	
	m_construct = renderer->ProduceVC( *this, vs );
}

bool VertexDeclaration::operator==( const VertexDeclaration & b ) const
{
	if ( GetSizeInBytes() != b.GetSizeInBytes() )
	{
		return false;
	}

	for ( size_t i = 0; i < m_allElements.size(); ++i )
	{
		if ( memcmp( &m_allElements[i], &b.m_allElements[i], sizeof( VertexElement ) ) != 0 )
		{
			return false;
		}
	}

	return true;
}

bool VertexDeclaration::operator!=( const VertexDeclaration & b ) const
{
	return !(operator==( b ));
}


void VertexDeclaration::Destroy()
{
	m_allElements.clear();
	m_elementMap.clear();
}

size_t VertexDeclaration::GetNumberOfElements() const
{
	return m_allElements.size();
}

bool VertexDeclaration::ElementExists( const std::string & name ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	return !(itr == m_elementMap.end());
}

bool VertexDeclaration::GetElement( const std::string name, VertexElement & element ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	return GetElement( itr->second.buffer, itr->second.item, element );
}

bool VertexDeclaration::GetElement( size_t buffer, size_t item, VertexElement & element ) const
{
	if ( buffer >= m_elements.size() ) return false;
	if ( item >= m_elements[buffer].size() ) return false;

	element = m_elements[buffer][item];
	return true;
}

bool VertexDeclaration::GetElement( VertexElement toFind, VertexElement & elementOut ) const
{
	for ( auto && buffer : m_elements )
	{
		for ( auto && element : buffer )
		{
			if ( unify::StringIs( element.SemanticName, toFind.SemanticName ) && element.SemanticIndex == toFind.SemanticIndex && element.InputSlot == toFind.InputSlot )
			{
				elementOut = element;
				return true;
			}
		}
	}
	return false;
}

size_t VertexDeclaration::GetSizeInBytes( size_t slot ) const
{
	return m_sizeInBytes[ slot ];
}

size_t VertexDeclaration::NumberOfSlots() const
{
	return m_numberOfSlots;
}

const std::vector< VertexElement > & VertexDeclaration::Elements() const
{
	return m_allElements;
}

const std::vector< VertexElement > & VertexDeclaration::Elements( size_t slot ) const
{
	return m_elements[ slot ];
}

int VertexDeclaration::GetInstanceingSlot() const
{
	return m_instanceingSlot;
}

Instancing::TYPE VertexDeclaration::GetInstancing( size_t slot ) const
{
	assert( slot <= m_instances.size() );
	return m_instancing[ slot ];
}

size_t VertexDeclaration::GetInstances( size_t slot ) const
{
	assert( slot <= m_instances.size() );
	return m_instances[ slot ];
}

void VertexDeclaration::Use()
{
	m_construct->Use();
}

