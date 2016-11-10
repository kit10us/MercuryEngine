// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Mercury.h>
#include <me/VertexElement.h>
#include <qxml/Element.h>
#include <qjson/QJson.h>
#include <unify/DataLock.h>
#include <memory>

namespace me 
{
	class IVertexShader;

	struct VertexDeclarationParameters
	{
		VertexDeclarationParameters()
			: xml( nullptr )
		{
		}

		VertexDeclarationParameters( const qxml::Element * _xml )
			: xml( _xml )
		{
		}

		VertexDeclarationParameters( qjson::Object _json )
			: xml( nullptr )
			, json( _json )
		{
		}

		const qxml::Element * xml;
		const qjson::Object json;
	};

	class IVertexDeclaration
	{
	public:
		typedef std::shared_ptr< IVertexDeclaration > ptr;

		virtual ~IVertexDeclaration() {}

		/// <summary>
		/// Build the vertex declaration against a shader. The shader is used to
		/// ensure the formating of the VD is correct.
		/// </summary>
		virtual void Build( const IVertexShader & vs ) = 0;

		/// <summary>
		/// Return the number of elements.
		/// </summary>
		virtual size_t GetNumberOfElements() const = 0;
		
		/// <summary>
		/// Returns true if an element with 'name' exists, ignores case.
		/// </summary>
		virtual bool ElementExists( const std::string & name ) const = 0;

		/// <summary>
		/// Get an element by name.
		/// </summary>
		virtual bool GetElement( const std::string name, VertexElement & element ) const = 0;

		/// <summary>
		/// Get an element by index.
		/// </summary>
		virtual bool GetElement( size_t index, VertexElement & element ) const = 0;

		/// <summary>
		/// Get an element that closely matches a specified element.
		/// </summary>
		virtual bool GetElement( VertexElement toFind, VertexElement & element ) const = 0;

		// Can throw, ensure ElementExists is tested before calling this function.
		virtual size_t GetElementOffset( const std::string & name ) const = 0;

		virtual size_t GetElementSize( const std::string & name ) const = 0;

		/// <summary>
		/// Returns the total size in bytes of the vertex this declaration declares, for a specific slot.
		/// </summary>
		virtual size_t GetSize( size_t slot ) const = 0;

		/// <summary>
		/// Returns the number of slots (buffers) this declaration uses.
		/// </summary>
		virtual size_t NumberOfSlots() const = 0;

		virtual std::vector< VertexElement > Elements() const = 0;

		virtual void Use() = 0;
	};
}