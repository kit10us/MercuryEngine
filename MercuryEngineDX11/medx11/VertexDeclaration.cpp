// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <medx11/VertexDeclaration.h>
#include <medx11/Renderer.h>
#include <medx11/VertexShader.h>
#include <atlbase.h>
#include <algorithm>

using namespace medx11;
using namespace me;

D3D11_INPUT_ELEMENT_DESC ToDX( VertexElement & element )
{
	D3D11_INPUT_ELEMENT_DESC out{};
	out.InputSlot = element.InputSlot;

	switch ( element.Format )
	{
	case ElementFormat::Float1: out.Format = DXGI_FORMAT_R32_FLOAT; break;
	case ElementFormat::Float2: out.Format = DXGI_FORMAT_R32G32_FLOAT; break;
	case ElementFormat::Float3: out.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
	case ElementFormat::Float4: out.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
	case ElementFormat::ColorUNorm: out.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
	case ElementFormat::Unknown: out.Format = DXGI_FORMAT_UNKNOWN; break;
	}

	out.SemanticName = element.SemanticName.c_str();
	out.SemanticIndex = element.SemanticIndex;
	out.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // element.AlignedByteOffset;				 +
	out.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

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

	CComPtr< ID3D11InputLayout > m_layout;

	Pimpl( VertexDeclaration & owner, me::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< Renderer * >(renderer) )
	{
	}

	Pimpl( VertexDeclaration & owner, me::IRenderer * renderer, VertexDeclarationParameters parameters )
		: Pimpl( owner, renderer )
	{
		if( parameters.xml )
		{
			// Accumulate elements.
			size_t totalSizeInBytes = 0; // Automatically increases based on asusmed element size.
			for( const auto child : parameters.xml->Children() )
			{
				if( child.IsTagName( "element" ) )
				{
					VertexElement element( child );
					element.AlignedByteOffset = totalSizeInBytes;

					std::string name = child.GetAttribute( "name" )->GetString();

					m_elementMap[name] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
					m_elements.push_back( element );

					totalSizeInBytes += element.SizeOf();
				}
			}
		}
		else if( ! parameters.json.IsNullOrEmpty() )
		{
			// Accumulate elements.
			size_t totalSizeInBytes = 0; // Automatically increases based on asusmed element size.
			for( auto itr : parameters.json )
				//for( const qxml::Element * itr = element.GetFirstChild(), *end = 0; itr != end; itr = itr->GetNext() )
			{
				const qjson::Object * member = dynamic_cast<const qjson::Object *>(itr.GetValue().get());
				if( member != nullptr )
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
		std::vector< D3D11_INPUT_ELEMENT_DESC > elements;
		for ( auto & e : m_elements )
		{
			elements.push_back( ToDX( e ) );
		}

		HRESULT hr = m_renderer->GetDxDevice()->CreateInputLayout( &elements[0], elements.size(), vs.GetBytecode(), vs.GetBytecodeLength(), &m_layout );
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
		return m_elements.size();
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
		m_renderer->GetDxContext()->IASetInputLayout( m_layout );
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

