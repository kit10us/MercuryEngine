// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/VertexDeclaration.h>
#include <me/IRenderer.h>
#include <me/IVertexShader.h>
#include <unify/ColorUnit.h>
#include <algorithm>

using namespace me;
									  
VertexDeclaration::VertexDeclaration()
	: m_instancing( Instancing::None )
	, m_numberOfSlots( 0 )
	, m_instanceingSlot{ -1 }
{
}

VertexDeclaration::VertexDeclaration( const qxml::Element * xml )
	: VertexDeclaration()
{
	const size_t maxSlots = 16;

	for( auto && slot : xml->Children( "slot" ) )
	{
		m_numberOfSlots++;
		m_sizeInBytes.push_back( 0 );
		m_slotElements.push_back( std::vector< me::VertexElement >() );
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
			m_elementMap[name] = m_allElements.size(); // The next index is size, so this is what we will reference from our map.
			m_allElements.push_back( vertexElement );
			m_slotElements[vertexElement.InputSlot].push_back( vertexElement );

			m_sizeInBytes[ vertexElement.InputSlot ] += vertexElement.SizeOf();
		}
	}

	/*
	m_instancing = Instancing::None;
	if ( xml->HasAttributes( "instancing" ) )
	{
		m_instancing = Instancing::FromString( xml->GetAttribute< std::string >( "instancing" ) );
	}																			 

	for ( const auto child : xml->Children() )
	{
		if ( child.IsTagName( "element" ) )
		{
			VertexElement element( child );
			element.AlignedByteOffset = totalSizeInBytes[ element.InputSlot ];

			std::string name = element.SemanticName;

			m_elementMap[name] = m_allElements.size(); // The next index is size, so this is what we will reference from our map.
			m_allElements.push_back( element );

			totalSizeInBytes[ element.InputSlot ] += element.SizeOf();
		}
	}
	*/

	if ( m_allElements.size() == 0 )
	{
		throw unify::Exception( "No vertex elements defined for vertex declaration!" );
	}
}

VertexDeclaration::VertexDeclaration( const qjson::Object json )
	: VertexDeclaration()
{
	const size_t maxSlots = 16;

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
				m_slotElements.resize( m_numberOfSlots, std::vector< me::VertexElement >() );
			}
	
			element.AlignedByteOffset = m_sizeInBytes[ element.InputSlot ];
			
			m_elementMap[name] = m_allElements.size(); // The next index is size, so this is what we will reference from our map.
			m_allElements.push_back( element );
			m_slotElements[ element.InputSlot ].push_back( element );
	
			m_sizeInBytes[ element.InputSlot ] += element.SizeOf();
		}
	}	

	if ( m_allElements.size() == 0 )
	{
		throw unify::Exception( "No vertex elements defined for vertex declaration!" );
	}
}

VertexDeclaration::~VertexDeclaration()
{
		Destroy();
}

void VertexDeclaration::Build( const IRenderer * renderer, const IVertexShader & vs )
{	
	m_construct = renderer->ProduceVC( *this, vs );
}

bool VertexDeclaration::operator==( const VertexDeclaration & b ) const
{
	if ( GetSizeInBytes() != b.GetSizeInBytes() || m_allElements.size() != b.m_allElements.size() )
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
	return GetElement( itr->second, element );
}

bool VertexDeclaration::GetElement( size_t index, VertexElement & element ) const
{
	if ( index < m_allElements.size() )
	{
		element = m_allElements[index];
		return true;
	}
	else
	{
		return false;
	}
}

bool VertexDeclaration::GetElement( VertexElement toFind, VertexElement & element ) const
{
	auto elementItr = std::find_if(
		m_allElements.begin(), m_allElements.end(),
		[ & ]( auto & e )
	{
		return unify::StringIs( e.SemanticName, toFind.SemanticName ) && e.SemanticIndex == toFind.SemanticIndex && e.InputSlot == toFind.InputSlot;
	} );
	if ( elementItr == m_allElements.end() )
	{
		return false;
	}
	element = *elementItr;
	return true;
}
						   
size_t VertexDeclaration::GetElementOffset( const std::string & name ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	if ( itr == m_elementMap.end() )
	{
		throw unify::Exception( "VertexDeclaration: Element not found!" );
	}

	return m_allElements[itr->second].AlignedByteOffset;
}

size_t VertexDeclaration::GetElementSize( const std::string & name ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	if ( itr == m_elementMap.end() )
	{
		throw unify::Exception( "VertexDeclaration: Element not found!" );
	}

	return m_allElements[itr->second].SizeOf();
}

size_t VertexDeclaration::GetSizeInBytes( size_t slot ) const
{
	size_t size = 0;
	for( auto e : m_allElements )
	{
		if( e.InputSlot == slot )
		{
			size += e.SizeOf();
		}
	}

	return size;
}

size_t VertexDeclaration::NumberOfSlots() const
{
	return m_numberOfSlots;
}

const std::vector< VertexElement > & VertexDeclaration::Elements() const
{
	return m_allElements;
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

