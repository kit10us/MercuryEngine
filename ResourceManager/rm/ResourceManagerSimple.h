// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

///
/// Resource management: Interfaces to simplify the management of resource files. There would be a main management interface with associated
/// individual -item- resource lists. For instance, sound, textures/surfaces, geometry will all have resource lists. They can be interdependant.
///

#pragma once

#include <qxml/Document.h>
#include <rm/ResourceManagerBase.h>
#include <unify/String.h>
#include <unify/Stream.h>
#include <unify/Exception.h>
#include <unify/Stream.h>
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
		virtual std::shared_ptr< T > Produce( unify::Path path, void * data ) = 0;
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

		bool Exists( std::string name ) const override;

		// Add an entry for a resource.
		ResourcePtr Add( std::string name, T * resource ) override;

		ResourcePtr Add( std::string name, unify::Path source, void * data = 0 );
	
		void AddResource( std::string name, unify::Path path ) override;

		// Find an existing resource.
		ResourcePtr Find( std::string name );

		size_t Count() const override;

        void ForEach( ForEachFunctor & functor );

		void AddFactory( std::string extension, std::shared_ptr< ISourceFactory< T > > factory );

	protected:
		std::string m_resourceName;
		std::map< std::string, ResourcePtr > m_resourceList; 
		std::map< std::string, std::shared_ptr< ISourceFactory< T > >, unify::CaseInsensitiveLessThanTest > m_sourceFactories;
	};

	#include <rm/ResourceManagerSimple.inl>
}