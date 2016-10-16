// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IRenderer.h>
#include <dxi/VertexElement.h>
#include <unify/unify.h>
#include <unify/String.h>
#include <unify/DataLock.h>
#include <qxml/Element.h>
#include <qjson/QJson.h>
#include <map>

namespace dxi
{
	/// <summary> 
	/// Defines the structure of the data in a vertex. 
	/// </summary>
	class VertexDeclaration
	{
	public:
		typedef std::shared_ptr< VertexDeclaration > ptr;

		VertexDeclaration( core::IRenderer * renderer );
		VertexDeclaration( core::IRenderer * renderer, const qxml::Element & element );
		VertexDeclaration( core::IRenderer * renderer, const qjson::Object & object );

		~VertexDeclaration();

		void Build( const class VertexShader & vs );

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

		class Pimpl;
		std::shared_ptr< Pimpl > m_pimpl;

		std::vector< VertexElement > m_elements;
		ElementMap m_elementMap;
		size_t m_totalSizeInBytes;
	};
}