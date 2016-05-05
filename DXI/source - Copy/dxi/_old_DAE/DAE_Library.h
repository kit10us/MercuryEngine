// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <qxml/QXML.h>
#include <dxi/dae/DAE_DocumentNode.h>
#include <dxi/dae/DAE_Exceptions.h>
#include <dxi/dae/DAE_InstanceSet.h>
#include <dxi/core/Game.h>
#include <boost/optional.hpp>
#include <map>
#include <vector>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_<T> - generic abstract for a library.
		/// </summary>
		template< typename T >
		class Library : public DocumentNode
		{
		public:
			Library( IDocument & document, const qxml::Element * node, const std::string & tagName );

			const std::string & GetID() const;
			const std::string & GetName() const;
			const std::vector< T * > & Ordered() const;
			const T * Find( const std::string & name ) const;

		private:
			std::string m_id;
			std::string m_name;

			// Library things are stored in a vector for ordered access, and a map for quick lookup.
			std::vector< T * > m_ordered;
			std::map< std::string, T * > m_lookup;
			std::vector< boost::shared_ptr< T > > m_managed; // Used to manage the memory (allows us to simplify the other containers).
		};

		template< typename T >
		Library< T >::Library( IDocument & document, const qxml::Element * node, const std::string & tagName )
			: DocumentNode( document, node )
			, m_id( node->GetStringAttributeElse( "id", std::string() ) )
			, m_name( node->GetStringAttributeElse( "name", std::string() ) )
		{
			for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
			{
				dxi::core::Game::GetGameInstance()->GetOS().DebugWriteLine( "Library for " + tagName + ": " + childNode->GetTagName() + " - \"" + childNode->GetStringAttribute( "id" ) + "\"" );
				if ( childNode->IsTagName( tagName ) )
				{
					T * thing = new T( document, childNode );
					if ( childNode->HasAttributes( "id" ) )
					{
						m_lookup[ childNode->GetStringAttribute( "id" ) ] = thing;
						GetDocument().Add( childNode->GetStringAttribute( "id" ), thing );
					}
					m_ordered.push_back( thing );
					m_managed.push_back( boost::shared_ptr< T >( thing ) );
				}
			}
		}

		template< typename T >
		const std::string & Library< T >::GetID() const
		{
			return m_id;
		}

		template< typename T >
		const std::string & Library< T >::GetName() const
		{
			return m_name;
		}

		template< typename T >
		const std::vector< T * > & Library< T >::Ordered() const
		{
			return m_ordered;
		}

		template< typename T >
		const T * Library< T >::Find( const std::string & name ) const
		{
			std::string realName;
			
			// Remove prepended "#" from name if it exists...
			if ( name.at( 0 ) == '#' )
			{
				realName = name.substr( 1 );
			}
			else
			{
				realName = name;
			}
			std::map< std::string, T * >::const_iterator itr = m_lookup.find( realName );
			return ( itr == m_lookup.end() ) ? 0 : itr->second;
		}
	}
}
