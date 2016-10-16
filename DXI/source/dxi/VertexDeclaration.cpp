// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/VertexDeclaration.h>
#include <dxi/win/DXRenderer.h>
#include <unify/ColorUnit.h>

using namespace dxi;

#if defined( DIRECTX9 )

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
	case ElementFormat::Color: out.Type = D3DDECLTYPE_D3DCOLOR; break;
	case ElementFormat::Unknown: out.Type = D3DDECLTYPE_UNUSED; break;
	}

	return out;
}

#elif defined( DIRECTX11 )

D3D11_INPUT_ELEMENT_DESC ToDX( VertexElement & elmenet )
{
	assert( 0 ); // TODO:
	VertexElement specularE = {};
	specularE.InputSlot = slot;
	specularE.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	specularE.SemanticName = "COLOR";
	specularE.SemanticIndex = 1;
	specularE.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	return specularE;
}
 
#endif
		   

class VertexDeclaration::Pimpl
{
public:	
	Pimpl( VertexDeclaration & owner, core::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< win::DXRenderer * >( renderer ) )
	{
	}

	~Pimpl()
	{
	}

	VertexDeclaration & m_owner;
	win::DXRenderer * m_renderer;

#if defined( DIRECTX9 )
	CComPtr< IDirect3DVertexDeclaration9 > m_layout;
#elif defined( DIRECTX11 )
	CComPtr< ID3D11InputLayout > m_layout;
#endif
};		

VertexDeclaration::VertexDeclaration( core::IRenderer * renderer )
	: m_pimpl( new Pimpl( *this, renderer ) )
{
}

VertexDeclaration::VertexDeclaration( core::IRenderer * renderer, const qxml::Element & element )
	: m_pimpl( new Pimpl( *this, renderer ) )
{
	// Accumulate elements.
	m_totalSizeInBytes = 0; // Automatically increases based on asusmed element size.
	for( const auto child : element.Children() )
	{
		if( child.IsTagName( "element" ) )
		{
			unsigned short slot = 0;
			if( child.HasAttributes( "stream" ) )
			{
				slot = child.GetAttributeElse< unsigned short >( "stream", 0 );
			}
			else if( child.HasAttributes( "slot" ) )
			{
				slot = child.GetAttributeElse< unsigned short >( "slot", 0 );
			}

			//unsigned short offset = child.GetAttributeElse< unsigned short >( "offset", m_totalSizeInBytes );

			ElementFormat format = ElementFormatFromString( child.GetAttribute( "type" )->GetString() );
			size_t size = SizeOf( format );

			std::string name = child.GetAttribute( "name" )->GetString();

			std::string usage{};
			unsigned char usageIndex = child.GetAttributeElse< unsigned char >( "usageindex", 0 );
			if( child.HasAttributes( "usage" ) )
			{
				usage = child.GetAttribute( "usage" )->GetString();
			}
			else // Get the usage from the name...
			{
				if ( unify::BeginsWith( name, "TEXCOORD" ) )
				{
					usage = "TEXCOORD";
					std::string n = name.substr( strlen( "TEXCOORD" ) );
					usageIndex = unify::Cast< unsigned char >( n );
				}
				else if ( unify::BeginsWith( name, "TEX" ) )
				{
					usage = "TEXCOORD";
					std::string n = name.substr( strlen( "TEX" ) );
					usageIndex = unify::Cast< unsigned char >( n );
				}
				else if ( unify::StringIs( name, "DIFFUSE" ) || unify::StringIs( name, "COLOR" ) )
				{
					usage = "COLOR";
				}
				else if ( unify::StringIs( name, "SPECULAR" ) )
				{
					usage = "COLOR";
					usageIndex = 1;
				}
				else
				{
					usage = name;
				}
			}

			VertexElement element{};
			element.InputSlot = slot;
			element.AlignedByteOffset = m_totalSizeInBytes;
			element.Format = format;
			element.SemanticName = usage;
			element.SemanticIndex = usageIndex;

			m_elementMap[name] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
			m_elements.push_back( element );

			m_totalSizeInBytes += size;
		}
	}

	if( m_elements.size() == 0 )
	{
		throw unify::Exception( "No vertex elements found for vertex declaration!" );
	}
}

VertexDeclaration::VertexDeclaration( core::IRenderer * renderer, const qjson::Object & object )
	: m_pimpl( new Pimpl( *this, renderer ) )
{
	// Accumulate elements.
	m_totalSizeInBytes = 0; // Automatically increases based on asusmed element size.
	for( auto itr : object )
		//for( const qxml::Element * itr = element.GetFirstChild(), *end = 0; itr != end; itr = itr->GetNext() )
	{
		size_t elementSizeInBytes = 0;
		const qjson::Object * member = dynamic_cast< const qjson::Object * >(itr.GetValue().get());
		if( member != nullptr )
		{
			assert( 0 ); //TODO: What?	
		}
		else // Simple string as type.
		{
			unsigned short slot = 0;
			ElementFormat format = ElementFormatFromString( itr.GetValue()->ToString() );
			size_t size = SizeOf( format );

			std::string name = itr.GetName();

			std::string usage{};
			unsigned char usageIndex = 0;
			if ( unify::BeginsWith( name, "TEXCOORD" ) )
			{
				usage = "TEXCOORD";
				std::string n = name.substr( strlen( "TEXCOORD" ) );
				usageIndex = unify::Cast< unsigned char >( n );
			}
			else if ( unify::BeginsWith( name, "TEX" ) )
			{
				usage = "TEXCOORD";
				std::string n = name.substr( strlen( "TEX" ) );
				usageIndex = unify::Cast< unsigned char >( n );
			}
			else if ( unify::StringIs( name, "DIFFUSE" ) || unify::StringIs( name, "COLOR" ) )
			{
				usage = "COLOR";
			}
			else if ( unify::StringIs( name, "SPECULAR" ) )
			{
				usage = "COLOR";
				usageIndex = 1;
			}
			else
			{
				usage = name;
			}

			VertexElement element{};
			element.InputSlot = slot;
			element.AlignedByteOffset = m_totalSizeInBytes;
			element.Format = format;
			element.SemanticName = usage.c_str();
			element.SemanticIndex = usageIndex;

			m_elementMap[name] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
			m_elements.push_back( element );

			elementSizeInBytes = size;
		}
		m_totalSizeInBytes += elementSizeInBytes;
	}

	if( m_elements.size() == 0 )
	{
		throw unify::Exception( "No vertex elements found for vertex declaration!" );
	}
}

VertexDeclaration::~VertexDeclaration()
{
	Destroy();
}

void VertexDeclaration::Build( const VertexShader & vs )
{	
#if defined( DIRECTX9 )
	std::vector< D3DVERTEXELEMENT9 > elements;

	for( auto & e : m_elements )
	{
		elements.push_back( ToDX( e ) );
	}

	elements.push_back( D3DDECL_END() );

	HRESULT hr = m_pimpl->m_renderer->GetDxDevice()->CreateVertexDeclaration( &elements[0], &m_pimpl->m_layout );
	if( FAILED( hr ) )
	{
		throw unify::Exception( "Failed to create vertex declaration!" );
	}
#elif defined( DIRECTX11 )
	// TODO:
	std::vector< D3D11_INPUT_ELEMENT_DESC > elements;
	for ( auto & e : m_elements )
	{
		elements.push_back( ToDX( e ) );
	}

	HRESULT hr = win::DX::GetDxDevice()->CreateInputLayout( &elements[0], elements.size(), vs.GetBytecode(), vs.GetBytecodeLength(), &m_layout );
	if( FAILED( hr ) )
	{
		throw unify::Exception( "Failed to create vertex declaration!" );
	}
#endif
}

bool VertexDeclaration::operator==( const VertexDeclaration & b ) const
{
	if( GetSize() != b.GetSize() || m_elements.size() != b.m_elements.size() )
	{
		return false;
	}

	for( size_t i = 0; i < m_elements.size(); ++i )
	{
		if( memcmp( &m_elements[i], &b.m_elements[i], sizeof( VertexElement ) ) != 0 )
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
	m_pimpl->m_layout = nullptr;
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
	if( index < m_elements.size() )
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
	if( elementItr == m_elements.end() )
	{
		return false;
	}
	element = *elementItr;
	return true;
}


size_t VertexDeclaration::GetElementOffset( const std::string & name ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	if( itr == m_elementMap.end() )
	{
		throw unify::Exception( "VertexDeclaration: Element not found!" );
	}

	return m_elements[itr->second].AlignedByteOffset;
}

size_t VertexDeclaration::GetElementSize( const std::string & name ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	if( itr == m_elementMap.end() )
	{
		throw unify::Exception( "VertexDeclaration: Element not found!" );
	}

	return SizeOf( m_elements[itr->second].Format );
}

size_t VertexDeclaration::GetSize() const
{
	return m_totalSizeInBytes;
}

void VertexDeclaration::Use()
{
#if defined( DIRECTX9 )
	HRESULT result = m_pimpl->m_renderer->GetDxDevice()->SetVertexDeclaration( m_pimpl->m_layout );
	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to set vertex declaration!" );
	}
#elif defined( DIRECTX11 )
	win::DX::GetDxContext()->IASetInputLayout( m_layout );
#endif
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V2< float > & v ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = v.x;
		destItem->y = v.y;
		destItem->z = 0;
		destItem->w = 0;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V3< float > & v ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V4< float > & v ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = v.x;
		destItem->y = v.y;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = v.x;
		destItem->y = v.y;
		destItem->z = v.z;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = v.x;
		destItem->y = v.y;
		destItem->z = v.z;
		destItem->w = v.w;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::TexCoords & tc ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = tc.u;
		destItem->y = tc.v;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = tc.u;
		destItem->y = tc.v;
		destItem->z = 0;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = tc.u;
		destItem->y = tc.v;
		destItem->z = 0;
		destItem->w = 0;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::Color & c ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	unify::ColorUnit cu( c );

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		destItem->w = cu.a;
		return true;
	}
	case ElementFormat::Color:
	{
		unify::Color * destItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = c;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::ColorUnit & cu ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	unify::Color c( cu );

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		destItem->w = cu.a;
		return true;
	}
	case ElementFormat::Color:
	{
		unify::Color * destItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = c;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, const VertexDeclaration & inVD, const void * vertex ) const
{
	size_t result = 0;
	for( auto e : inVD.m_elements )
	{
		size_t offset = e.AlignedByteOffset;
		switch(  e.Format )
		{
		case ElementFormat::Float2:
		{
			unify::V2< float > * d = reinterpret_cast<unify::V2< float > *>((unsigned char *)vertex + offset);
			result += WriteVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case ElementFormat::Float3:
		{
			unify::V3< float > * d = reinterpret_cast<unify::V3< float > *>((unsigned char *)vertex + offset);
			result += WriteVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
		}
		break;
		case ElementFormat::Float4:
		{
			unify::V4< float > * d = reinterpret_cast<unify::V4< float > *>((unsigned char *)vertex + offset);
			result += WriteVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case ElementFormat::Color:
		{
			unify::Color * d = reinterpret_cast<unify::Color *>((unsigned char *)vertex + offset);
			result += WriteVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		default:
			break;
		}
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V2< float > & v )  const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, element, v ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V3< float > & v ) const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, element, v ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V4< float > & v ) const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, element, v ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::TexCoords & tc ) const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, element, tc ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::Color & c ) const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, element, c ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::ColorUnit & cu ) const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, element, cu ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, const VertexDeclaration & inVD, const void * vertex ) const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, inVD, vertex ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V2< float > & v )	const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * sourceItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v = *sourceItem;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v = *sourceItem;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V3< float > & v )	const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;
	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		const unify::V2< float > * sourceItem = reinterpret_cast<const unify::V2< float > *>((const unsigned char *)lock.GetItemReadOnly( vertexIndex ) + offset);
		v = *sourceItem;
		return true;
	}
	case ElementFormat::Float3:
	{
		const unify::V3< float > * sourceItem = reinterpret_cast<const unify::V3< float > *>((const unsigned char *)lock.GetItemReadOnly( vertexIndex ) + offset);
		v = *sourceItem;
		return true;
	}
	case ElementFormat::Float4:
	{
		const unify::V4< float > * sourceItem = reinterpret_cast<const unify::V4< float > *>((const unsigned char *)lock.GetItemReadOnly( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		v.z = sourceItem->z;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V4< float > & v ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * sourceItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		v.z = sourceItem->z;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		v.z = sourceItem->z;
		v.w = sourceItem->w;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::TexCoords & tc ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * sourceItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		tc.u = sourceItem->x;
		tc.v = sourceItem->y;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		tc.u = sourceItem->x;
		tc.v = sourceItem->y;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		tc.u = sourceItem->x;
		tc.v = sourceItem->y;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::Color & c ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	unify::ColorUnit cu( c );

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		cu.a = sourceItem->w;
		return true;
	}
	case ElementFormat::Color:
	{
		unify::Color * sourceItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		c = *sourceItem;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::ColorUnit & cu ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	unify::Color c( cu );

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		cu.a = sourceItem->w;
		return true;
	}
	case ElementFormat::Color:
	{
		unify::Color * sourceItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		c = *sourceItem;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, const VertexDeclaration & inVD, void * vertex )	const
{
	size_t result = 0;
	for( auto e : inVD.m_elements )
	{
		size_t offset = e.AlignedByteOffset;
		switch(  e.Format )
		{
		case ElementFormat::Float2:
		{
			unify::V2< float > * d = reinterpret_cast<unify::V2< float > *>((unsigned char *)vertex + offset);
			result += ReadVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case ElementFormat::Float3:
		{
			unify::V3< float > * d = reinterpret_cast<unify::V3< float > *>((unsigned char *)vertex + offset);
			result += ReadVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
		}
		break;
		case ElementFormat::Float4:
		{
			unify::V4< float > * d = reinterpret_cast<unify::V4< float > *>((unsigned char *)vertex + offset);
			result += ReadVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case ElementFormat::Color:
		{
			unify::Color * d = reinterpret_cast<unify::Color *>((unsigned char *)vertex + offset);
			result += ReadVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		default:
			break;
		}
	}
	return result != 0;
}



