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
	class ElementList;
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

		int GetIntegerAttribute( const std::string & attribute ) const;
		float GetFloatAttribute( const std::string & attribute ) const;
		std::string GetStringAttribute( const std::string & attribute ) const;
		bool GetBooleanAttribute( const std::string & attribute ) const;
		
		template< typename T >
		T GetAttribute( const std::string & attribute ) const;

		template< typename T >
		T GetAttributeElse( const std::string & attribute, T value ) const;

		int GetIntegerAttributeElse( const std::string & attribute, int value ) const;
		float GetFloatAttributeElse( const std::string & attribute, float value ) const;
		std::string GetStringAttributeElse( const std::string & attribute, const std::string & value ) const;
		bool GetBooleanAttributeElse( const std::string & attribute, bool value ) const;

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
        const Element * FindFirstElement( const std::string tagName, const std::string & attributes = std::string() ) const;
		void FindElementsByTagName( ElementList & elementList, const std::string & tagName );
		void FindElementsByTagNameRecursive( ElementList & elementList, const std::string & tagName );
		Element * GetElement( const std::string & tagName );
		const Element * GetElement( const std::string & tagName ) const;
		NodeType::TYPE GetType() const;
        Document * GetDocument() const;

		class iterator
		{
		public:
			iterator( Element * element )
				: m_element( element )
			{
			}

			iterator & operator++()
			{
				m_element = m_element->GetNext();
				return *this;
			}

			bool operator!=( const iterator & itr ) const
			{
				return m_element != itr.m_element;
			}

			const Element & operator*()
			{
				return *m_element;
			}

		private:
			Element * m_element;
		};

		iterator begin() {
			return iterator( m_firstChild );
		}
		iterator end() {
			return iterator( m_lastChild + 1 );
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


	class ElementList : public unify::LinkList< Element >
	{ 
	public:
		Element * AddElement( Element * pElement );
	};

	#include <qxml/Element.inl>
}