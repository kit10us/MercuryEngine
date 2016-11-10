// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <medx9/VertexDeclaration.h>
#include <medx9/Renderer.h>
#include <medx9/VertexShader.h>
#include <unify/ColorUnit.h>
#include <atlbase.h>
#include <algorithm>

using namespace medx9;
using namespace me;

D3DVERTEXELEMENT9 ToDX( VertexElement element )
{
	D3DVERTEXELEMENT9 out {};
	
	if ( unify::StringIs( "POSITION", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_POSITION;
	}
	else if ( unify::StringIs( "BLENDWEIGHT", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_BLENDWEIGHT;
	}
	else if ( unify::StringIs( "BLENDINDICES", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_BLENDINDICES;
	}
	else if ( unify::StringIs( "NORMAL", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_NORMAL;
	}
	else if ( unify::StringIs( "PSIZE", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_PSIZE;
	}
	else if ( unify::StringIs( "TEXCOORD", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_TEXCOORD;
	}
	else if ( unify::StringIs( "TANGENT", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_TANGENT;
	}
	else if ( unify::StringIs( "BINORMAL", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_BINORMAL;
	}
	else if ( unify::StringIs( "TESSFACTOR", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_TESSFACTOR;
	}
	else if ( unify::StringIs( "POSITIONT", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_POSITIONT;
	}
	else if ( unify::StringIs( "COLOR", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_COLOR;
	}
	else if ( unify::StringIs( "FOG", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_FOG;
	}
	else if ( unify::StringIs( "DEPTH", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_DEPTH;
	}
	else if ( unify::StringIs( "SAMPLE", element.SemanticName ) )
	{
		out.Usage = D3DDECLUSAGE_SAMPLE;
	}
	else
	{
		throw unify::Exception( "Failed to convert string Vertex Declaration usage!" );
	}
			 	
	out.UsageIndex = element.SemanticIndex;
	out.Offset = element.AlignedByteOffset;
	out.Stream = element.InputSlot;

	switch ( element.Format )
	{
	case ElementFormat::Float1: out.Type = D3DDECLTYPE_FLOAT1; break;
	case ElementFormat::Float2: out.Type = D3DDECLTYPE_FLOAT2; break;
	case ElementFormat::Float3: out.Type = D3DDECLTYPE_FLOAT3; break;
	case ElementFormat::Float4: out.Type = D3DDECLTYPE_FLOAT4; break;
	case ElementFormat::ColorUNorm: out.Type = D3DDECLTYPE_D3DCOLOR; break;
	case ElementFormat::Unknown: out.Type = D3DDECLTYPE_UNUSED; break;
	}

	return out;
}



class VertexDeclaration::Pimpl
{
public:	
	VertexDeclaration & m_owner;
	Renderer * m_renderer;

	// ElementMap: first, std::string, is the element's name for lookup, and second, size_t, is the index into m_elements.
	typedef std::map< std::string, size_t, unify::CaseInsensitiveLessThanTest > ElementMap;

	std::vector< VertexElement > m_elements;
	ElementMap m_elementMap;
	size_t m_totalSizeInBytes;

	CComPtr< IDirect3DVertexDeclaration9 > m_layout;

	Pimpl( VertexDeclaration & owner, me::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< Renderer * >( renderer ) )
	{
	}

	Pimpl( VertexDeclaration & owner, me::IRenderer * renderer, VertexDeclarationParameters parameters )
		: Pimpl( owner, renderer )
	{
		if( parameters.xml )
		{
			m_totalSizeInBytes = 0; // Automatically increases based on asusmed element size.
			for ( const auto child : parameters.xml->Children() )
			{
				if ( child.IsTagName( "element" ) )
				{
					VertexElement element( child );
					element.AlignedByteOffset = m_totalSizeInBytes;

					std::string name = child.GetAttribute( "name" )->GetString();

					m_elementMap[name] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
					m_elements.push_back( element );

					m_totalSizeInBytes += element.SizeOf();
				}
			}
		}
		else if ( ! parameters.json.IsNullOrEmpty() )
		{
			m_totalSizeInBytes = 0; // Automatically increases based on asusmed element size.
			for ( auto itr : parameters.json )
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
	
					element.AlignedByteOffset = m_totalSizeInBytes;
	
					m_elementMap[name] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
					m_elements.push_back( element );
	
					m_totalSizeInBytes += element.SizeOf();
				}
			}	
		}

		if ( m_elements.size() == 0 )
		{
			throw unify::Exception( "No vertex elements found for vertex declaration!" );
		}
	}

	~Pimpl()
	{
		Destroy();
	}

	void Build( const IVertexShader & vs )
	{
		std::vector< D3DVERTEXELEMENT9 > elements;

		for ( auto & e : m_elements )
		{
			elements.push_back( ToDX( e ) );
		}

		elements.push_back( D3DDECL_END() );

		HRESULT hr = m_renderer->GetDxDevice()->CreateVertexDeclaration( &elements[0], &m_layout );
		if ( FAILED( hr ) )
		{
			throw unify::Exception( "Failed to create vertex declaration!" );
		}
	}

	void Destroy()
	{
		m_elements.clear();
		m_elementMap.clear();
		m_layout = nullptr;
	}

	size_t GetNumberOfElements() const
	{
		return m_elementMap.size(); // Note that the size of m_elements is either 0 or the number of elements + 1.
	}

	bool ElementExists( const std::string & name ) const
	{
		ElementMap::const_iterator itr = m_elementMap.find( name );
		return !(itr == m_elementMap.end());
	}

	bool GetElement( const std::string name, VertexElement & element ) const
	{
		ElementMap::const_iterator itr = m_elementMap.find( name );
		return GetElement( itr->second, element );
	}

	bool GetElement( size_t index, VertexElement & element ) const
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

	bool GetElement( VertexElement toFind, VertexElement & element ) const
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

	size_t GetElementOffset( const std::string & name ) const
	{
		ElementMap::const_iterator itr = m_elementMap.find( name );
		if ( itr == m_elementMap.end() )
		{
			throw unify::Exception( "VertexDeclaration: Element not found!" );
		}

		return m_elements[itr->second].AlignedByteOffset;
	}

	size_t GetElementSize( const std::string & name ) const
	{
		ElementMap::const_iterator itr = m_elementMap.find( name );
		if ( itr == m_elementMap.end() )
		{
			throw unify::Exception( "VertexDeclaration: Element not found!" );
		}

		return m_elements[itr->second].SizeOf();
	}

	size_t GetSize( size_t slot ) const
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

	size_t NumberOfSlots() const
	{
		size_t numberOfSlots = 0;
		for( auto e : m_elements )
		{
			numberOfSlots = std::max( numberOfSlots, e.InputSlot );
		}
		return numberOfSlots;
	}

	void Use()
	{
		HRESULT result = m_renderer->GetDxDevice()->SetVertexDeclaration( m_layout );
		if ( FAILED( result ) )
		{
			throw unify::Exception( "Failed to set vertex declaration!" );
		}
	}
};		


VertexDeclaration::VertexDeclaration( me::IRenderer * renderer )
	: m_pimpl( new Pimpl( *this, renderer ) )
{
}

VertexDeclaration::VertexDeclaration( me::IRenderer * renderer, VertexDeclarationParameters parameters )
	: m_pimpl( new Pimpl( *this, renderer, parameters ) )
{
}

VertexDeclaration::~VertexDeclaration()
{
}

void VertexDeclaration::Build( const IVertexShader & vs )
{	
	m_pimpl->Build( vs );
}

bool VertexDeclaration::operator==( const VertexDeclaration & b ) const
{
	if ( GetSize() != b.GetSize() || m_pimpl->m_elements.size() != b.m_pimpl->m_elements.size() )
	{
		return false;
	}

	for ( size_t i = 0; i < m_pimpl->m_elements.size(); ++i )
	{
		if ( memcmp( &m_pimpl->m_elements[i], &b.m_pimpl->m_elements[i], sizeof( VertexElement ) ) != 0 )
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
	m_pimpl->Destroy();
}

size_t VertexDeclaration::GetNumberOfElements() const
{
	return m_pimpl->GetNumberOfElements();
}

bool VertexDeclaration::ElementExists( const std::string & name ) const
{
	return m_pimpl->ElementExists( name );
}

bool VertexDeclaration::GetElement( const std::string name, VertexElement & element ) const
{
	return m_pimpl->GetElement( name, element );
}

bool VertexDeclaration::GetElement( size_t index, VertexElement & element ) const
{
	return m_pimpl->GetElement( index, element );
}

bool VertexDeclaration::GetElement( VertexElement toFind, VertexElement & element ) const
{
	return m_pimpl->GetElement( toFind, element );
}
						   
size_t VertexDeclaration::GetElementOffset( const std::string & name ) const
{
	return m_pimpl->GetElementOffset( name );
}

size_t VertexDeclaration::GetElementSize( const std::string & name ) const
{
	return m_pimpl->GetElementSize( name );
}

size_t VertexDeclaration::GetSize( size_t slot ) const
{
	return m_pimpl->GetSize( slot );
}

size_t VertexDeclaration::NumberOfSlots() const
{
	return m_pimpl->NumberOfSlots();
}

std::vector< VertexElement > VertexDeclaration::Elements() const
{
	return m_pimpl->m_elements;
}

void VertexDeclaration::Use()
{
	m_pimpl->Use();
}

