// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/unify.h>
#include <unify/String.h>
#include <unify/DataLock.h>
#include <unify/V2.h>
#include <unify/V3.h>
#include <unify/V4.h>
#include <unify/TexCoords.h>
#include <unify/Color.h>
#include <dxi/win/DirectX.h>
#include <qxml/Element.h>
#include <qjson/QJson.h>
#include <map>
#include <atlbase.h>

namespace dxi
{
#ifdef DX11
	typedef D3D11_INPUT_ELEMENT_DESC VertexElement;
#else
	typedef D3DVERTEXELEMENT9 VertexElement;
#endif

	/// <summary> 
	/// Defines the structure of the data in a vertex. 
	/// </summary>
	/// <notes>
	/// Replaces FVF use, which was legacy DX8 vertex format specification that was superceeded by Vertex Declarations in DX9, and completely obsolete by DX10+.
	/// </notes>
	class VertexDeclaration
	{
	public:
		VertexDeclaration();
		VertexDeclaration( const VertexDeclaration & from );
		VertexDeclaration( const qxml::Element & element );
		VertexDeclaration( const qjson::Object & object );
		VertexDeclaration & operator=( const VertexDeclaration & from );

		~VertexDeclaration();

		bool operator==( const VertexDeclaration & b ) const;
		bool operator!=( const VertexDeclaration & b ) const;

		void Destroy();
		
		size_t GetNumberOfElements() const;
		bool ElementExists( const std::string & name ) const;

		/// <summary>
		/// Get an element by name.
		/// </summary>
		bool GetElement( const std::string name, VertexElement & element ) const;

		/// <summary>
		/// Get an element by index.
		/// </summary>
		bool GetElement( size_t index, VertexElement & element ) const;

		/// <summary>
		/// Get an element that closely matches a specified element.
		/// </summary>
		bool GetElement( VertexElement toFind, VertexElement & element ) const;

		// Can throw, ensure ElementExists is tested before calling this function.
		size_t GetElementOffset( const std::string & name ) const;
		size_t GetElementSize( const std::string & name ) const;

		/// Returns the total size in bytes of the vertex this declaration declares.
		size_t GetSize() const;

		void Use();

		bool WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V2< float > & v ) const;
		bool WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V3< float > & v ) const;
		bool WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V4< float > & v ) const;
		bool WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::TexCoords & tc ) const;
		bool WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::Color & c ) const;
		bool WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::ColorUnit & cu ) const;
		bool WriteVertex( unify::DataLock & lock, size_t vertexIndex, const VertexDeclaration & inVD, const void * vertex ) const;

		bool WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V2< float > & v ) const;
		bool WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V3< float > & v ) const;
		bool WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V4< float > & v ) const;
		bool WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::TexCoords & tc ) const;
		bool WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::Color & c ) const;
		bool WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::ColorUnit & cu ) const;
		bool WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, const VertexDeclaration & inVD, const void * vertex ) const;

		bool ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V2< float > & v ) const;
		bool ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V3< float > & v ) const;
		bool ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V4< float > & v ) const;
		bool ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::TexCoords & tc ) const;
		bool ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::Color & c ) const;
		bool ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::ColorUnit & cu ) const;
		bool ReadVertex( unify::DataLock & lock, size_t vertexIndex, const VertexDeclaration & inVD, void * vertex ) const;

	private:
		// ElementMap: first, std::string, is the element's name for lookup, and second, size_t, is the index into m_elements.
		typedef std::map< std::string, size_t, unify::CaseInsensitiveLessThanTest > ElementMap;

		std::vector< VertexElement > m_elements;
		ElementMap m_elementMap;
		CComPtr< IDirect3DVertexDeclaration9 > m_vertexDeclaration;
		size_t m_totalSizeInBytes;
	};
}