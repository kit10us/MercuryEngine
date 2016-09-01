// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

///
/// Resource management: Interfaces to simplify the management of resource files. There would be a main management interface with associated
/// individual -item- resource lists. For instance, sound, textures/surfaces, geometry will all have resource lists. They can be interdependant.
///

#pragma once

#include <unify/Stream.h>
#include <unify/Exception.h>
#include <dxi/exception/FailedToCreate.h>
#include <qjson/Object.h>
#include <qxml/Element.h>
#include <rm/ResourceManagerBase.h>
#include <map>
#include <memory>

namespace rm
{
	/// <summary>
	/// Factory from a source path.
	/// </summary>
	template< typename T >
	class ISourceFactory
	{
	public:
		~ISourceFactory() {}
		virtual T * Produce( unify::Path path ) = 0;
	};

	/// <summary> 
	/// Factory from a Json object.
	/// </summary>
	template< typename T >
	class IJsonFactory
	{
	public:
		~IJsonFactory() {}
		virtual T * Produce( qjson::Object json ) = 0;
	};

	/// <summary>
	/// Factory from an XML node.
	/// </summary>
	template< typename T >
	class IXMLFactory
	{
	public:
		~IXMLFactory() {}
		virtual T * Produce( unify::Path path ) = 0;
		virtual T * Produce( const qxml::Element * node ) = 0;
	};

	// A base for a resource list...
	template< typename T >
	class ResourceManagerSimple	: public IResourceManager< T >
	{
	private:
		ResourceManagerSimple( const ResourceManagerSimple& ) {}
		ResourceManagerSimple& operator=( const ResourceManagerSimple& ) {}

	public:
        class ForEachFunctor 
        {
        public:
            virtual ~ForEachFunctor() {}
            virtual void operator()( T & resource ) = 0;    
        };

		typedef std::shared_ptr< T > ResourcePtr;

        ResourceManagerSimple( std::string resourceName );
		~ResourceManagerSimple();

		std::string GetName() const override;

		void Clear();

		bool Exists( const ResourceID & id ) const override;

		// Add an entry for a resource.
		ResourcePtr Add( const ResourceID & id, T * resource ) override;

		ResourcePtr Add( const ResourceID & id, unify::Path source );
		
		ResourcePtr Add( qjson::Object json );
		
		ResourcePtr Add( const qxml::Element * node );

		// Find an existing resource.
		ResourcePtr Find( const ResourceID & id );

		size_t Count() const override;

        void ForEach( ForEachFunctor & functor );

		void AddFactory( IJsonFactory< T > * factory );
		void AddFactory( ISourceFactory< T > * factory );
		void AddFactory( IXMLFactory< T > * factory );

	protected:
		std::string m_resourceName;
		std::map< ResourceID, ResourcePtr > m_resourceList; 
		std::list< std::shared_ptr< IJsonFactory< T > > > m_jsonFactories;
		std::list< std::shared_ptr< ISourceFactory< T > > > m_sourceFactories;
		std::list< std::shared_ptr< IXMLFactory< T > > > m_xmlFactories;
	};

	#include <rm/ResourceManagerSimple.inl>
}