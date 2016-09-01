// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/Attribute.h>
#include <unify/FStream.h>
#include <unify/LinkList.h>
#include <unify/unify.h>
#include <vector>
#include <list>

namespace qxml
{
    class Document;

	class Element
	{
		friend class Document;
	public:
		struct NodeType
		{
			enum TYPE
			{
				Element,
				Comment,	
				CDATA,
				Command,
				Text
			};
		};

		Element();
		Element( const std::string & name, NodeType::TYPE type, Document * document );
		~Element();

		const std::string & GetTagName() const;

		// Case insensitive tag name test.
		bool IsTagName( const std::string & tagName ) const;
		unsigned int NumAttributes() const;
		bool HasAttributes( const std::string & name ) const;
		bool HasElements( const std::string & name ) const;
		Attribute::shared_ptr GetAttribute( unsigned int attribute ) const;
		Attribute::shared_ptr GetAttribute( const std::string & attribute ) const;
	
		template< typename T >
		T GetAttribute( const std::string & attribute ) const;

		template< typename T >
		T GetAttributeElse( const std::string & attribute, T value ) const;

		Element * GetParent();
		const Element * GetParent() const;
		Element * GetPrevious();
		const Element * GetPrevious() const;
		Element * GetNext();
		const Element * GetNext() const;
		Element * GetFirstChild();
		const Element * GetFirstChild() const;
		Element * GetLastChild();
		const Element * GetLastChild() const;
		const std::string & GetText() const;
		unsigned int NumChildren() const;
		unsigned int Index() const;
		void AppendAttribute( Attribute::shared_ptr & pAttribute );
		const std::string & AddText( const std::string & text );
		void TakeChild( Element * pElement );
		void TakeSibling( Element * pElement );
        
		void FindElements( std::list< const Element * > & elementList, const std::string tagName, const std::string & attributes = std::string() ) const;
		void FindElementsRecursive( std::list< const Element * > & elementList, const std::string tagName, const std::string & attributes = std::string() ) const;
		
		const Element * FindFirstElement( const std::string tagName, const std::string & attributes = std::string() ) const;
		Element * GetElement( const std::string & tagName );
		const Element * GetElement( const std::string & tagName ) const;
		NodeType::TYPE GetType() const;
        Document * GetDocument() const;

		template< typename T >
		class iterate_children
		{
		public:
			iterate_children( T & owner, std::string tagName )
				: m_owner( owner ), m_tagName( tagName )
			{
			}

			class iterator
			{
			public:
				iterator( T * element, std::string tagName )
					: m_element( element ), m_tagName( tagName )
				{
					if( !m_tagName.empty() )
					{
						while( m_element != nullptr && ! m_element->IsTagName( m_tagName ) ) m_element = m_element->GetNext();
					}
				}

				iterator & operator++()
				{
					m_element = m_element->GetNext();
					if ( ! m_tagName.empty() )
					{
						while( m_element != nullptr && ! m_element->IsTagName( m_tagName ) ) m_element = m_element->GetNext();
					}
					return *this;
				}

				bool operator!=( const iterator & itr ) const
				{
					return m_element != itr.m_element;
				}

				const T & operator*()
				{
					return *m_element;
				}

			private:
				T * m_element;
				std::string m_tagName;
			};

			iterator begin() 
			{
				return iterator( m_owner.GetFirstChild(), m_tagName );
			}
			iterator end() {
				return iterator( nullptr, m_tagName );
			}

		private:
			T & m_owner;
			std::string m_tagName;
		};

		// TODO: add XPath support, instead of tagName (which would also add attribute culling/incl. support).
		iterate_children< Element > Children( std::string tagName = std::string() )
		{
			return iterate_children< Element > ( *this, tagName );
		}

		iterate_children< const Element > Children( std::string tagName = std::string() ) const
		{
			return iterate_children< const Element >( *this, tagName );
		}


		class iterate_attributes
		{
		public:
			iterate_attributes( std::vector< Attribute::shared_ptr >::iterator begin, std::vector< Attribute::shared_ptr >::iterator end )
				: m_begin( begin )
				, m_end( end )
			{
			}

			class iterator
			{
			public:
				iterator( std::vector< Attribute::shared_ptr >::iterator itr )
					: m_itr( itr )
				{
				}

				iterator & operator++()
				{
					m_itr++;
					return *this;
				}

				bool operator!=( const iterator & itr ) const
				{
					return m_itr != itr.m_itr;
				}

				const Attribute & operator*()
				{
					return *(*m_itr).get();
				}

			private:
				std::vector< Attribute::shared_ptr >::iterator m_itr;
			};

			iterator begin() {
				return iterator( m_begin );
			}
			iterator end() {
				return iterator( m_end );
			}

		private:
			std::vector< Attribute::shared_ptr >::iterator m_begin;
			std::vector< Attribute::shared_ptr >::iterator m_end;
		};

		iterate_attributes Attributes()
		{
			return iterate_attributes( m_attributeList.begin(), m_attributeList.end() );
		}

	protected:
		std::string m_tagName;
		std::vector< Attribute::shared_ptr > m_attributeList;
		std::string m_data;
		std::string m_text;
		unsigned int m_index;
		NodeType::TYPE m_type;

		Element * m_parent;
		Element * m_prevSibling;
		Element * m_nextSibling;
		Element * m_firstChild;
		Element * m_lastChild;

        Document * m_document;
		
		unsigned int m_numChildren;
	};
		
	#include <qxml/Element.inl>

	// TODO: Remove
	class ElementList : public unify::LinkList< Element >
	{
	public:
		Element * AddElement( Element * pElement );
	};

}

#include <qxml/AttributeCast.h>
