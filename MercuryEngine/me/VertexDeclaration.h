// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IVertexConstruct.h>
#include <me/VertexElement.h>
#include <me/Instancing.h>
#include <unify/unify.h>
#include <unify/String.h>
#include <unify/DataLock.h>
#include <map>

namespace me
{
	class IRenderer;

	/// <summary> 
	/// Defines the structure of the data in a vertex. 
	/// </summary>
	class VertexDeclaration
	{
	public:
		typedef std::shared_ptr< VertexDeclaration > ptr;

		VertexDeclaration();
		VertexDeclaration( const qxml::Element * xml );
		VertexDeclaration( const qjson::Object json );

		~VertexDeclaration();

		void Build( class IRenderer * renderer, const IVertexShader & vs );
		
		bool operator==( const VertexDeclaration & b ) const;
		bool operator!=( const VertexDeclaration & b ) const;

		void Destroy();
		
		size_t GetNumberOfElements() const;
		bool ElementExists( const std::string & name ) const;

		/// <summary>
		/// Get an element by name.
		/// </summary>
		bool GetElement( const std::string name, me::VertexElement & element ) const;

		/// <summary>
		/// Get an element by buffer and item index.
		/// </summary>
		bool GetElement( size_t buffer, size_t item, me::VertexElement & element ) const;

		/// <summary>
		/// Get an element that closely matches a specified element.
		/// </summary>
		bool GetElement( me::VertexElement toFind, me::VertexElement & element ) const;

		size_t GetSizeInBytes( size_t slot = 0 ) const;

		size_t NumberOfSlots() const;

		const std::vector< me::VertexElement > & Elements() const;

		const std::vector< me::VertexElement > & Elements( size_t slot ) const;

		int GetInstanceingSlot() const;
		me::Instancing::TYPE GetInstancing( size_t slot ) const;
		size_t GetInstances( size_t slot ) const;

		void Use();

	private:
		// ElementMap: first, std::string, is the element's name for lookup, and second, size_t, is the index into m_elements.
		struct BufferItem {
			size_t buffer, item;
		};
		typedef std::map< std::string, BufferItem, unify::CaseInsensitiveLessThanTest > ElementMap;								  

		size_t m_numberOfSlots;
		std::vector< me::VertexElement > m_allElements;
		std::vector< std::vector< me::VertexElement > > m_elements;
		ElementMap m_elementMap;
		std::vector< size_t > m_sizeInBytes;
		std::vector< me::Instancing::TYPE > m_instancing;
		std::vector< size_t > m_instances;
		int m_instanceingSlot;

		me::IVertexConstruct::ptr m_construct;
	};
}