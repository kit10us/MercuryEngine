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
#include <unify/AssetPaths.h>
#include <map>
#include <memory>

namespace rm
{
	class ILogger
	{
	public:
		typedef std::shared_ptr< ILogger > ptr;

		~ILogger() {}

		virtual void WriteLine( std::string text ) = 0;
	};


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

        ResourceManagerSimple( std::string resourceName, unify::AssetPaths * assetPaths = nullptr, ILogger::ptr = ILogger::ptr() );
		~ResourceManagerSimple();

		std::string GetName() const override;

		void Clear();

		bool Exists( std::string name ) const override;

		// Add an entry for a resource.
		ResourcePtr Add( std::string name, T * resource ) override;

		ResourcePtr Add( std::string name, unify::Path source, unify::Path relativePath = "", void * data = 0 );
	
		void AddResource( std::string name, unify::Path path ) override;

		/// <summary>
		/// Find an existing resource by name.
		/// </summary>
		ResourcePtr Find( std::string name );

		/// <summary>
		/// Get an existing resource by index.
		/// </summary>
		ResourcePtr Get( size_t index );

		size_t Count() const override;

		std::string GetResourceName( size_t index ) const override;

        void ForEach( ForEachFunctor & functor );

		void AddFactory( std::string extension, std::shared_ptr< ISourceFactory< T > > factory );

		void Log_Write( std::string text );

		void Log_WriteLine( std::string text );

	protected:
		std::string m_resourceName;
		unify::AssetPaths * m_assetPaths;
		ILogger::ptr m_logger;
		std::map< std::string, ResourcePtr > m_resourceMap;
		std::vector< ResourcePtr > m_resourceList;
		std::vector< std::string > m_resourceNames;
		std::map< std::string, std::shared_ptr< ISourceFactory< T > >, unify::CaseInsensitiveLessThanTest > m_sourceFactories;
	};

	#include <rm/ResourceManagerSimple.inl>
}