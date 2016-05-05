// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

///
/// Resource management: Interfaces to simplify the management of resource files. There would be a main management interface with associated
/// individual -item- resource lists. For instance, sound, textures/surfaces, geometry will all have resource lists. They can be interdependant.
///

#pragma once

#include <dxi/Resource.h>
#include <dxi/ResourceOwner.h>
#include <boost/utility.hpp>
#include <dxi/ResourceDesc.h>
#include <unify/Stream.h>
#include <unify/Exception.h>
#include <qjson/Object.h>
#include <map>
#include <memory>

namespace dxi
{
	typedef std::string ResourceID;

	// A base for a resource list...
	template< typename T >
	class ResourceManager : public boost::noncopyable
	{
	public:
		typedef std::shared_ptr< T > ResourcePtr;

        class ForEachFunctor 
        {
        public:
            virtual ~ForEachFunctor() {}
            virtual void operator()( T & resource ) = 0;    
        };

		class IFactory
		{
		public:
			~IFactory() {}
			virtual T * Produce( const qjson::Object & json ) = 0;
		};

        ResourceManager( const ResourceDesc & resourceDesc, std::shared_ptr< IFactory > factory );
		virtual ~ResourceManager();

		virtual void Clear();

		virtual bool Exists( const ResourceID & id ) const;

		// Add an entry for a resource.
		virtual ResourcePtr Add( const ResourceID & id, T * resource );

		virtual ResourcePtr Add( const ResourceID & id, std::string source );

		// Find an existing resource.
		virtual ResourcePtr Find( const ResourceID & id );

		ResourceDesc & GetResourceDesc();

		size_t Count() const;

        void ForEach( ForEachFunctor & functor );

	protected:
		std::map< ResourceID, ResourcePtr > m_resourceList; 
		dxi::ResourceDesc m_resourceDesc;
		std::shared_ptr< IFactory > m_factory;
	};

	#include <dxi/ResourceManager.inl>
}