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
{
}

VertexDeclaration::VertexDeclaration( const qxml::Element * xml )
	: VertexDeclaration()
{
	size_t totalSizeInBytes = 0; // Automatically increases based on asusmed element size.
	for ( const auto child : xml->Children() )
	{
		if ( child.IsTagName( "element" ) )
		{
			VertexElement element( child );
			element.AlignedByteOffset = totalSizeInBytes;

			std::string name = child.GetAttribute( "name" )->GetString();

			m_elementMap[name] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
			m_elements.push_back( element );

			totalSizeInBytes += element.SizeOf();
		}
	}

	if ( m_elements.size() == 0 )
	{
		throw unify::Exception( "No vertex elements found for vertex declaration!" );
	}
}

VertexDeclaration::VertexDeclaration( const qjson::Object json )
	: VertexDeclaration()
{
	size_t totalSizeInBytes = 0; // Automatically increases based on asusmed element size.
	for ( auto itr : json )
		//for( const qxml::Element * itr = element.GetFirstChild(), *end = 0; itr != end; itr = itr->GetNext() )
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
	
			element.AlignedByteOffset = totalSizeInBytes;
	
			m_elementMap[name] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
			m_elements.push_back( element );
	
			totalSizeInBytes += element.SizeOf();
		}
	}	

	if ( m_elements.size() == 0 )
	{
		throw unify::Exception( "No vertex elements found for vertex declaration!" );
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
	if ( GetSize() != b.GetSize() || m_elements.size() != b.m_elements.size() )
	{
		return false;
	}

	for ( size_t i = 0; i < m_elements.size(); ++i )
	{
		if ( memcmp( &m_elements[i], &b.m_elements[i], sizeof( VertexElement ) ) != 0 )
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
	m_elements.clear();
	m_elementMap.clear();
}

size_t VertexDeclaration::GetNumberOfElements() const
{
	return m_elementMap.size(); // Note that the size of m_elements is either 0 or the number of elements + 1.
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
	if ( index < m_elements.size() )
	{
		element = m_elements[index];
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
		m_elements.begin(), m_elements.end(),
		[ & ]( auto & e )
	{
		return unify::StringIs( e.SemanticName, toFind.SemanticName ) && e.SemanticIndex == toFind.SemanticIndex;
	} );
	if ( elementItr == m_elements.end() )
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

	return m_elements[itr->second].AlignedByteOffset;
}

size_t VertexDeclaration::GetElementSize( const std::string & name ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	if ( itr == m_elementMap.end() )
	{
		throw unify::Exception( "VertexDeclaration: Element not found!" );
	}

	return m_elements[itr->second].SizeOf();
}

size_t VertexDeclaration::GetSize( size_t slot ) const
{
	size_t size = 0;
	for( auto e : m_elements )
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
	size_t numberOfSlots = 0;
	for( auto e : m_elements )
	{
		numberOfSlots = std::max( numberOfSlots, e.InputSlot );
	}
	return numberOfSlots;
}

const std::vector< VertexElement > & VertexDeclaration::Elements() const
{
	return m_elements;
}

Instancing::TYPE VertexDeclaration::GetInstancing() const
{
	return m_instancing;
}

void VertexDeclaration::Use()
{
	m_construct->Use();
}

