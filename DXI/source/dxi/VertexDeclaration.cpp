// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/VertexDeclaration.h>
#include <dxi/core/Game.h>
#include <unify/ColorUnit.h>

using namespace dxi;

// Convert from a string to a D3DDECLTYPE. Used to enable loading from text based sources.
D3DDECLTYPE ConvertVertexDeclarationType( const std::string & type )
{
	if ( unify::StringIs( "FLOAT1", type ) || unify::StringIs( "FLOAT", type ) )
	{
		return D3DDECLTYPE_FLOAT1;
	}
	else if ( unify::StringIs( "FLOAT2", type ) || unify::StringIs( "TEXCOORD", type ) )
	{
		return D3DDECLTYPE_FLOAT2;
	}
	else if ( unify::StringIs( "FLOAT3", type ) )
	{
		return D3DDECLTYPE_FLOAT3;
	}
	else if ( unify::StringIs( "FLOAT4", type ) || unify::StringIs( "COLORUNIT", type ) )
	{
		return D3DDECLTYPE_FLOAT4;
	}
	else if ( unify::StringIs( "D3DCOLOR", type ) || unify::StringIs( "COLOR", type ) )
	{
		return D3DDECLTYPE_D3DCOLOR;
	}
	else if ( unify::StringIs( "UBYTE4", type ) )
	{
		return D3DDECLTYPE_UBYTE4;
	}
	else if ( unify::StringIs( "SHORT2", type ) )
	{
		return D3DDECLTYPE_SHORT2;
	}
	else if ( unify::StringIs( "SHORT4", type ) )
	{
		return D3DDECLTYPE_SHORT4;
	}
	else if ( unify::StringIs( "UBYTE4N", type ) )
	{
		return D3DDECLTYPE_UBYTE4N;
	}
	else if ( unify::StringIs( "SHORT2N", type ) )
	{
		return D3DDECLTYPE_SHORT2N;
	}
	else if ( unify::StringIs( "SHORT4N", type ) )
	{
		return D3DDECLTYPE_SHORT4N;
	}
	else if ( unify::StringIs( "USHORT2N", type ) )
	{
		return D3DDECLTYPE_USHORT2N;
	}
	else if ( unify::StringIs( "USHORT4N", type ) )
	{
		return D3DDECLTYPE_USHORT4N;
	}
	/*
	Deprecated in DirectX 10
	else if ( unify::StringIs( "UDEC3", type ) )
	{
		return D3DDECLTYPE_UDEC3;
	}
	else if ( unify::StringIs( "DEC3N", type ) )
	{
		return D3DDECLTYPE_DEC3N;
	}
	*/
	else if ( unify::StringIs( "FLOAT16_2", type ) )
	{
		return D3DDECLTYPE_FLOAT16_2;
	}
	else if ( unify::StringIs( "FLOAT16_4", type ) )
	{
		return D3DDECLTYPE_FLOAT16_4;
	}
	else if ( unify::StringIs( "UNUSED", type ) )
	{
		return D3DDECLTYPE_UNUSED;
	}
	else
	{
		throw unify::Exception( "Failed to convert string Vertex Declaration Type!" );
	}
}

D3DDECLMETHOD ConvertVertexDeclarationMethod( const std::string & method )
{
	if ( unify::StringIs( "DEFAULT", method ) )
	{
		return D3DDECLMETHOD_DEFAULT;
	}
	else if (  unify::StringIs( "PARTIALU", method ) )
	{
		return D3DDECLMETHOD_PARTIALU;
	}
	else if (  unify::StringIs( "PARTIALV", method ) )
	{
		return D3DDECLMETHOD_PARTIALV;
	}
	else if (  unify::StringIs( "CROSSUV", method ) )
	{
		return D3DDECLMETHOD_CROSSUV;
	}
	else if (  unify::StringIs( "UV", method ) )
	{
		return D3DDECLMETHOD_UV;
	}
	else if (  unify::StringIs( "LOOKUP", method ) )
	{
		return D3DDECLMETHOD_LOOKUP;
	}
	else if (  unify::StringIs( "LOOKUPPRESAMPLED", method ) )
	{
		return D3DDECLMETHOD_LOOKUPPRESAMPLED;
	}
	else
	{
		throw unify::Exception( "Failed to convert string Vertex Declaration Method!" );
	}
}

D3DDECLUSAGE ConvertVertexDeclarationUsage( const std::string & usage )
{
	if ( unify::StringIs( "POSITION", usage ) )
	{
		return D3DDECLUSAGE_POSITION;
	}
	else if (  unify::StringIs( "BLENDWEIGHT", usage ) )
	{
		return D3DDECLUSAGE_BLENDWEIGHT;
	}
	else if (  unify::StringIs( "BLENDINDICES", usage ) )
	{
		return D3DDECLUSAGE_BLENDINDICES;
	}
	else if (  unify::StringIs( "NORMAL", usage ) )
	{
		return D3DDECLUSAGE_NORMAL;
	}
	else if (  unify::StringIs( "PSIZE", usage ) )
	{
		return D3DDECLUSAGE_PSIZE;
	}
	else if (  unify::StringIs( "TEXCOORD", usage ) )
	{
		return D3DDECLUSAGE_TEXCOORD;
	}
	else if (  unify::StringIs( "TANGENT", usage ) )
	{
		return D3DDECLUSAGE_TANGENT;
	}
	else if (  unify::StringIs( "BINORMAL", usage ) )
	{
		return D3DDECLUSAGE_BINORMAL;
	}
	else if (  unify::StringIs( "TESSFACTOR", usage ) )
	{
		return D3DDECLUSAGE_TESSFACTOR;
	}
	else if (  unify::StringIs( "POSITIONT", usage ) )
	{
		return D3DDECLUSAGE_POSITIONT;
	}
	else if (  unify::StringIs( "COLOR", usage ) )
	{
		return D3DDECLUSAGE_COLOR;
	}
	else if (  unify::StringIs( "FOG", usage ) )
	{
		return D3DDECLUSAGE_FOG;
	}
	else if (  unify::StringIs( "DEPTH", usage ) )
	{
		return D3DDECLUSAGE_DEPTH;
	}
	else if (  unify::StringIs( "SAMPLE", usage ) )
	{
		return D3DDECLUSAGE_SAMPLE;
	}
	else
	{
		throw unify::Exception( "Failed to convert string Vertex Declaration usage!" );
	}
}

std::string VertexElementToName( D3DDECLUSAGE usage, unsigned char usageIndex )
{
	switch( usage )
	{
	case D3DDECLUSAGE_POSITION:
		return "POSITION";
	case D3DDECLUSAGE_BLENDWEIGHT:
		return "BLENDWEIGHT";
	case D3DDECLUSAGE_BLENDINDICES:
		return "BLENDINDICES";
	case D3DDECLUSAGE_NORMAL:
		return "NORMAL";
	case D3DDECLUSAGE_PSIZE:
		return "PSIZE";
	case D3DDECLUSAGE_TEXCOORD:
		return std::string( "TEXCOORD" ) + unify::Cast< std::string >( usageIndex );
	case D3DDECLUSAGE_TANGENT:
		return "TANGENT";
	case D3DDECLUSAGE_BINORMAL:
		return "BINORMAL";
	case D3DDECLUSAGE_TESSFACTOR:
		return "TESSFACTOR";
	case D3DDECLUSAGE_POSITIONT:
		return "POSITIONT";
	case D3DDECLUSAGE_COLOR:
		switch( usageIndex )
		{
		case 0:
		default:
			return "DIFFUSE";
		case 1:
			return "SPECULAR";
		}
	case D3DDECLUSAGE_FOG:
		return "FOG";
	case D3DDECLUSAGE_DEPTH:
		return "DEPTH";
	case D3DDECLUSAGE_SAMPLE:
		return "SAMPLE";
	default:
		throw unify::Exception( "Failed to convert string Vertex Declaration usage!" );
	}
}

void VertexNameToElement( const std::string & name, D3DDECLUSAGE & usageOut, unsigned char & usageIndexOut )
{
	if ( unify::StringIs( name, "POSITION" ) )
	{
		usageOut = D3DDECLUSAGE_POSITION;
		usageIndexOut = 0;
	}
	else if ( unify::StringIs( name, "BLENDWEIGHT" ) )
	{
		usageOut = D3DDECLUSAGE_BLENDWEIGHT;
		usageIndexOut = 0;
	}
	else if ( unify::StringIs( name, "BLENDINDICES" ) )
	{
		usageOut = D3DDECLUSAGE_BLENDINDICES;
		usageIndexOut = 0;
	}
	else if ( unify::StringIs( name, "NORMAL" ) )
	{
		usageOut = D3DDECLUSAGE_NORMAL;
		usageIndexOut = 0;
	}
	else if ( unify::StringIs( name, "PSIZE" ) )
	{
		usageOut = D3DDECLUSAGE_PSIZE;
		usageIndexOut = 0;
	}
	else if ( unify::BeginsWith( name, "TEXCOORD" ) )
	{
		usageOut = D3DDECLUSAGE_TEXCOORD;
		std::string n = name.substr( strlen( "TEXCOORD" ) ); 
		usageIndexOut = unify::Cast< unsigned char >( n );
	}
	else if( unify::BeginsWith( name, "TEX" ) )
	{
		usageOut = D3DDECLUSAGE_TEXCOORD;
		std::string n = name.substr( strlen( "TEX" ) );
		usageIndexOut = unify::Cast< unsigned char >( n );
	}
	else if ( unify::StringIs( name, "TANGENT" ) )
	{
		usageOut = D3DDECLUSAGE_TANGENT;
		usageIndexOut = 0;
	}
	else if ( unify::StringIs( name, "BINORMAL" ) )
	{
		usageOut = D3DDECLUSAGE_BINORMAL;
		usageIndexOut = 0;
	}
	else if ( unify::StringIs( name, "TESSFACTOR" ) )
	{
		usageOut = D3DDECLUSAGE_TESSFACTOR;
		usageIndexOut = 0;
	}
	else if ( unify::StringIs( name, "POSITIONT" ) )
	{
		usageOut = D3DDECLUSAGE_POSITIONT;
		usageIndexOut = 0;
	}
	else if ( unify::StringIs( name, "DIFFUSE" ) || unify::StringIs( name, "COLOR" ) )
	{
		usageOut = D3DDECLUSAGE_COLOR;
		usageIndexOut = 0;
	}
	else if ( unify::StringIs( name, "SPECULAR" ) )
	{
		usageOut = D3DDECLUSAGE_COLOR;
		usageIndexOut = 1;
	}
	else if ( unify::StringIs( name, "FOG" ) )
	{
		usageOut = D3DDECLUSAGE_FOG;
		usageIndexOut = 0;
	}
	else if ( unify::StringIs( name, "DEPTH" ) )
	{
		usageOut = D3DDECLUSAGE_DEPTH;
		usageIndexOut = 0;
	}
	else if ( unify::StringIs( name, "SAMPLE" ) )
	{
		usageOut = D3DDECLUSAGE_SAMPLE;
		usageIndexOut = 0;
	}
	else
	{
		throw unify::Exception( "Failed to convert string Vertex Declaration usage!" );
	}
}

// This is the automatically assumed size of the
size_t SizeOfVertexDeclarationElement( D3DDECLTYPE type )
{
	switch( type )
	{
	case D3DDECLTYPE_FLOAT1:
		return sizeof( float );
	case D3DDECLTYPE_FLOAT2:
		return sizeof( float ) * 2;
	case D3DDECLTYPE_FLOAT3:
		return sizeof( float ) * 3;
	case D3DDECLTYPE_FLOAT4:
		return sizeof( float ) * 4;
	case D3DDECLTYPE_D3DCOLOR:
		return sizeof( D3DCOLOR );
	case D3DDECLTYPE_UBYTE4:
		return sizeof( unsigned char ) * 4;
	case D3DDECLTYPE_SHORT2:
		return sizeof( short ) * 2;
	case D3DDECLTYPE_SHORT4:
		return sizeof( short ) * 4;
	case D3DDECLTYPE_UBYTE4N:
		return sizeof( unsigned char ) * 4;
	case D3DDECLTYPE_SHORT2N:
		return sizeof( short ) * 2;
	case D3DDECLTYPE_SHORT4N:
		return sizeof( short ) * 4;
	case D3DDECLTYPE_USHORT2N:
		return sizeof( unsigned short ) * 2;
	case D3DDECLTYPE_USHORT4N:
		return sizeof( unsigned short ) * 4;
	/* Deprecated in DirectX 10
	case D3DDECLTYPE_UDEC3:
	case D3DDECLTYPE_DEC3N:
	*/
	case D3DDECLTYPE_FLOAT16_2:
		return 2 * 2;
	case D3DDECLTYPE_FLOAT16_4:
		return 2 * 4;
	case D3DDECLTYPE_UNUSED:
		return 0;
	default:
		throw unify::Exception( "Vertex declaration type not support!" );
	}
}

VertexDeclaration::VertexDeclaration()
: m_vertexDeclaration( 0 )
{
}

VertexDeclaration::VertexDeclaration( const VertexDeclaration & from )
: m_vertexDeclaration( from.m_vertexDeclaration )
, m_elements( from.m_elements )
, m_elementMap( from.m_elementMap )
, m_totalSizeInBytes( from.m_totalSizeInBytes )
{
}

VertexDeclaration::VertexDeclaration( const qxml::Element & element )
{
	// Accumulate elements.
	m_totalSizeInBytes = 0; // Automatically increases based on asusmed element size.
	for( const auto child : element.Children() )
	{
		if ( child.IsTagName( "element" ) )
		{
			unsigned short stream = child.GetAttributeElse< unsigned short >( "stream", 0 );
			//unsigned short offset = child.GetAttributeElse< unsigned short >( "offset", m_totalSizeInBytes );
			D3DDECLTYPE type = ConvertVertexDeclarationType( child.GetAttribute( "type" )->GetString() );
			D3DDECLMETHOD method = child.HasAttributes( "method" ) ? ConvertVertexDeclarationMethod( child.GetAttribute( "method" )->GetString() ) : D3DDECLMETHOD_DEFAULT;

			std::string name = child.GetAttribute( "name" )->GetString();

			D3DDECLUSAGE usage = D3DDECLUSAGE();

			unsigned char usageIndex = child.GetAttributeElse< unsigned char >( "usageindex", 0 );

			if ( child.HasAttributes( "usage" ) )
			{
				usage = ConvertVertexDeclarationUsage( child.GetAttribute( "usage" )->GetString() );
			}
			else // Get the usage from the name...
			{
				VertexNameToElement( name, usage, usageIndex );
			}

			VertexElement element;
			element.Stream = stream;
			element.Offset = (WORD)m_totalSizeInBytes; //offset;
			element.Type = type;
			element.Method = method;
			element.Usage = usage;
			element.UsageIndex = usageIndex;
			m_elementMap[ name ] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
			m_elements.push_back( element );

			size_t size = SizeOfVertexDeclarationElement( type );
			//m_totalSizeInBytes = offset + size;
			m_totalSizeInBytes += size;
		}
	}

	if ( m_elements.size() == 0 )
	{
		throw unify::Exception( "No vertex elements found for vertex declaration!" );
	}

	VertexElement end = D3DDECL_END();
	m_elements.push_back( end );

	HRESULT hr = win::DX::GetDxDevice()->CreateVertexDeclaration( &m_elements[ 0 ], &m_vertexDeclaration );
	if ( FAILED( hr ) )
	{
		throw unify::Exception( "Failed to create vertex declaration!" );
	}
}

VertexDeclaration::VertexDeclaration( const qjson::Object & object )
{
	// Accumulate elements.
	m_totalSizeInBytes = 0; // Automatically increases based on asusmed element size.
	for( auto itr : object )
	//for( const qxml::Element * itr = element.GetFirstChild(), *end = 0; itr != end; itr = itr->GetNext() )
	{
		size_t elementSizeInBytes = 0;
		const qjson::Object * member = dynamic_cast< const qjson::Object * >( itr.GetValue().get() );
		if( member != nullptr )
		{
			int x( 0 ); x;
			/*
			unsigned short stream = itr.GetAttributeElse< unsigned short >( "stream", 0 );
			//unsigned short offset = itr->GetAttributeElse< unsigned short >( "offset", m_totalSizeInBytes );
			D3DDECLTYPE type = ConvertVertexDeclarationType( itr->GetAttribute( "type" )->GetString() );
			D3DDECLMETHOD method = itr->HasAttributes( "method" ) ? ConvertVertexDeclarationMethod( itr->GetAttribute( "method" )->GetString() ) : D3DDECLMETHOD_DEFAULT;
			D3DDECLUSAGE usage = ConvertVertexDeclarationUsage( itr->GetAttribute( "usage" )->GetString() );
			unsigned char usageIndex = itr->GetAttributeElse< unsigned char >( "usageindex", 0 );
			std::string name = element.HasAttributes( "name" ) ? element.GetAttribute( "name" )->GetString() : VertexElementToName( usage, usageIndex );

			VertexElement element;
			element.Stream = stream;
			element.Offset = (WORD)m_totalSizeInBytes; //offset;
			element.Type = type;
			element.Method = method;
			element.Usage = usage;
			element.UsageIndex = usageIndex;
			m_elementMap[name] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
			m_elements.push_back( element );

			size = SizeOfVertexDeclarationElement( type );
			*/
		}
		else // Simple string as type.
		{
			unsigned short stream = 0;
			D3DDECLTYPE type = ConvertVertexDeclarationType( itr.GetValue()->ToString() );
			D3DDECLMETHOD method = D3DDECLMETHOD_DEFAULT;
			unsigned char usageIndex = 0;
			std::string name = itr.GetName();
			
			D3DDECLUSAGE usage = D3DDECLUSAGE();
			VertexNameToElement( name, usage, usageIndex );

			VertexElement element;
			element.Stream = stream;
			element.Offset = (WORD)m_totalSizeInBytes; //offset;
			element.Type = type;
			element.Method = method;
			element.Usage = usage;
			element.UsageIndex = usageIndex;
			m_elementMap[name] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
			m_elements.push_back( element );

			elementSizeInBytes = SizeOfVertexDeclarationElement( type );
		}
		//m_totalSizeInBytes = offset + size;
		m_totalSizeInBytes += elementSizeInBytes;
	}

	if( m_elements.size() == 0 )
	{
		throw unify::Exception( "No vertex elements found for vertex declaration!" );
	}

	VertexElement end = D3DDECL_END();
	m_elements.push_back( end );

	HRESULT hr = win::DX::GetDxDevice()->CreateVertexDeclaration( &m_elements[0], &m_vertexDeclaration );
	if( FAILED( hr ) )
	{
		throw unify::Exception( "Failed to create vertex declaration!" );
	}
}

VertexDeclaration & VertexDeclaration::operator=( const VertexDeclaration & from )
{
	m_vertexDeclaration = from.m_vertexDeclaration;
	m_elements = from.m_elements;
	m_elementMap = from.m_elementMap;
	m_totalSizeInBytes = from.m_totalSizeInBytes;
	return *this;
}

VertexDeclaration::~VertexDeclaration()
{
	Destroy();
}

bool VertexDeclaration::operator==( const VertexDeclaration & b ) const
{
	if ( GetSize() != b.GetSize() || m_elements.size() != b.m_elements.size() ) 
	{
		return false;
	}

	for( size_t i = 0; i < m_elements.size(); ++i )
	{
		if ( m_elements[ i ].Method != b.m_elements[ i ].Method || 
			m_elements[ i ].Offset != b.m_elements[ i ].Offset ||
			m_elements[ i ].Stream != b.m_elements[ i ].Stream ||
			m_elements[ i ].Type != b.m_elements[ i ].Type ||
			m_elements[ i ].Usage != b.m_elements[ i ].Usage || 
			m_elements[ i ].UsageIndex != b.m_elements[ i ].UsageIndex )
		{
			return false;
		}
	}

	return true;
}

bool VertexDeclaration::operator!=( const VertexDeclaration & b ) const
{
	return !( operator==( b ) );
}

void VertexDeclaration::Destroy()
{
	m_elements.clear();
	m_elementMap.clear();

	m_vertexDeclaration.Release();
	m_vertexDeclaration = 0;
}

size_t VertexDeclaration::GetNumberOfElements() const
{
	return m_elementMap.size(); // Note that the size of m_elements is either 0 or the number of elements + 1.
}

bool VertexDeclaration::ElementExists( const std::string & name ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	return ! ( itr == m_elementMap.end() );
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
	auto elementItr = std::find_if( m_elements.begin(), m_elements.end(), [&]( auto & e ) { return e.Usage == toFind.Usage && e.UsageIndex == toFind.UsageIndex; } );
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
	if ( itr == m_elementMap.end() )
	{
		throw unify::Exception( "VertexDeclaration: Element not found!" );
	}
	return m_elements[ itr->second ].Offset;
}

size_t VertexDeclaration::GetElementSize( const std::string & name ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	if ( itr == m_elementMap.end() )
	{
		throw unify::Exception( "VertexDeclaration: Element not found!" );
	}
	return SizeOfVertexDeclarationElement( static_cast< D3DDECLTYPE >( m_elements[ itr->second ].Type ) );
}

size_t VertexDeclaration::GetSize() const
{
	return m_totalSizeInBytes;
}

void VertexDeclaration::Use()
{
	HRESULT result = win::DX::GetDxDevice()->SetVertexDeclaration( m_vertexDeclaration );
	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to set vertex declaration!" );
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V2< float > & v ) const
{
	VertexElement elementFound = {};
	if( ! GetElement( element, elementFound ) )
	{
		return false;
	}

	switch( elementFound.Type )
	{
	case D3DDECLTYPE_FLOAT2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		*destItem = v;
		return true;
	}
	case D3DDECLTYPE_FLOAT3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		*destItem = v;
		return true;
	}
	case D3DDECLTYPE_FLOAT4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
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

	switch( elementFound.Type )
	{
	case D3DDECLTYPE_FLOAT2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		*destItem = v;
		return true;
	}
	case D3DDECLTYPE_FLOAT3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		*destItem = v;
		return true;
	}
	case D3DDECLTYPE_FLOAT4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
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

	switch( elementFound.Type )
	{
	case D3DDECLTYPE_FLOAT2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		destItem->x = v.x;
		destItem->y = v.y;
		return true;
	}
	case D3DDECLTYPE_FLOAT3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		destItem->x = v.x;
		destItem->y = v.y;
		destItem->z = v.z;
		return true;
	}
	case D3DDECLTYPE_FLOAT4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
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

	switch( elementFound.Type )
	{
	case D3DDECLTYPE_FLOAT2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		destItem->x = tc.u;
		destItem->y = tc.v;
		return true;
	}
	case D3DDECLTYPE_FLOAT3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		destItem->x = tc.u;
		destItem->y = tc.v;
		destItem->z = 0;
		return true;
	}
	case D3DDECLTYPE_FLOAT4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
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

	switch( elementFound.Type )
	{
	case D3DDECLTYPE_FLOAT3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		return true;
	}
	case D3DDECLTYPE_FLOAT4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		destItem->w = cu.a;
		return true;
	}
	case D3DDECLTYPE_D3DCOLOR:
	{
		unify::Color * destItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
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

	switch( elementFound.Type )
	{
	case D3DDECLTYPE_FLOAT3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		return true;
	}
	case D3DDECLTYPE_FLOAT4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		destItem->w = cu.a;
		return true;
	}
	case D3DDECLTYPE_D3DCOLOR:
	{
		unify::Color * destItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
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
		switch( e.Type )
		{
		case D3DDECLTYPE_FLOAT2:
		{
			unify::V2< float > * d = reinterpret_cast<unify::V2< float > *>((unsigned char *)vertex + e.Offset);
			result += WriteVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case D3DDECLTYPE_FLOAT3:
		{
			unify::V3< float > * d = reinterpret_cast<unify::V3< float > *>((unsigned char *)vertex + e.Offset);
			result += WriteVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
		}
		break;
		case D3DDECLTYPE_FLOAT4:
		{
			unify::V4< float > * d = reinterpret_cast<unify::V4< float > *>((unsigned char *)vertex + e.Offset);
			result += WriteVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case D3DDECLTYPE_D3DCOLOR:
		{
			unify::Color * d = reinterpret_cast<unify::Color *>((unsigned char *)vertex + e.Offset);
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

	switch( elementFound.Type )
	{
	case D3DDECLTYPE_FLOAT2:
	{
		unify::V2< float > * sourceItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		v = *sourceItem;
		return true;
	}
	case D3DDECLTYPE_FLOAT3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		v = *sourceItem;
		return true;
	}
	case D3DDECLTYPE_FLOAT4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
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

	switch( elementFound.Type )
	{
	case D3DDECLTYPE_FLOAT2:
	{
		const unify::V2< float > * sourceItem = reinterpret_cast<const unify::V2< float > *>((const unsigned char *)lock.GetItemReadOnly( vertexIndex ) + elementFound.Offset);
		v = *sourceItem;
		return true;
	}
	case D3DDECLTYPE_FLOAT3:
	{
		const unify::V3< float > * sourceItem = reinterpret_cast<const unify::V3< float > *>((const unsigned char *)lock.GetItemReadOnly( vertexIndex ) + elementFound.Offset);
		v = *sourceItem;
		return true;
	}
	case D3DDECLTYPE_FLOAT4:
	{
		const unify::V4< float > * sourceItem = reinterpret_cast<const unify::V4< float > *>((const unsigned char *)lock.GetItemReadOnly( vertexIndex ) + elementFound.Offset);
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

	switch( elementFound.Type )
	{
	case D3DDECLTYPE_FLOAT2:
	{
		unify::V2< float > * sourceItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		return true;
	}
	case D3DDECLTYPE_FLOAT3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		v.z = sourceItem->z;
		return true;
	}
	case D3DDECLTYPE_FLOAT4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
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

	switch( elementFound.Type )
	{
	case D3DDECLTYPE_FLOAT2:
	{
		unify::V2< float > * sourceItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		tc.u = sourceItem->x;
		tc.v = sourceItem->y;
		return true;
	}
	case D3DDECLTYPE_FLOAT3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		tc.u = sourceItem->x;
		tc.v = sourceItem->y;
		return true;
	}
	case D3DDECLTYPE_FLOAT4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
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

	switch( elementFound.Type )
	{
	case D3DDECLTYPE_FLOAT3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		return true;
	}
	case D3DDECLTYPE_FLOAT4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		cu.a = sourceItem->w;
		return true;
	}
	case D3DDECLTYPE_D3DCOLOR:
	{
		unify::Color * sourceItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
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

	switch( elementFound.Type )
	{
	case D3DDECLTYPE_FLOAT3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		return true;
	}
	case D3DDECLTYPE_FLOAT4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		cu.a = sourceItem->w;
		return true;
	}
	case D3DDECLTYPE_D3DCOLOR:
	{
		unify::Color * sourceItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + elementFound.Offset);
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
		switch( e.Type )
		{
		case D3DDECLTYPE_FLOAT2:
		{
			unify::V2< float > * d = reinterpret_cast<unify::V2< float > *>((unsigned char *)vertex + e.Offset);
			result += ReadVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case D3DDECLTYPE_FLOAT3:
		{
			unify::V3< float > * d = reinterpret_cast<unify::V3< float > *>((unsigned char *)vertex + e.Offset);
			result += ReadVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
		}
		break;
		case D3DDECLTYPE_FLOAT4:
		{
			unify::V4< float > * d = reinterpret_cast<unify::V4< float > *>((unsigned char *)vertex + e.Offset);
			result += ReadVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case D3DDECLTYPE_D3DCOLOR:
		{
			unify::Color * d = reinterpret_cast<unify::Color *>((unsigned char *)vertex + e.Offset);
			result += ReadVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		default:
			break;
		}
	}
	return result != 0;
}
