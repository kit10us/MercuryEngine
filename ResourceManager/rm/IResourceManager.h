#pragma once

#include <rm/ResourcePack.h>
#include <rm/ISourceFactoryFunctor.h>
#include <unify/Path.h>
#include <map>
#include <list>
#include <string>
#include <memory>

namespace rm
{
	/// <summary>
	/// Used to reference a resouce.
	/// </summary>
	typedef std::string ResourceID;

	class IResourceManagerEarly
	{
	public:
		// Enable VTable...
		virtual ~IResourceManagerEarly() {}

		/// <summary>
		/// Returns the name of the resource being managed.
		/// </summary>
		virtual std::string GetName() const = 0;
	};

	template< typename T >
	class IResourceManager : public IResourceManagerEarly
	{
	public:
		typedef std::shared_ptr< T > ResourcePtr;

		/// <summary>
		/// Add resource with a specific ID.
		/// Throws an exception if a resource already exists with the same ID, since we are passing in an already allocated resource,
		/// this could lead to a unmanaged resource pointer.
		/// </summary>
		virtual ResourcePtr Add( const ResourceID & id, T * resource ) = 0;

		/// <summary>
		/// Returns true if a resource exists with the specified ID.
		/// </summary>
		virtual bool Exists( const ResourceID & id ) const = 0;

		/*
		/// <summary>
		/// Find and return a resource.
		/// </summary>
		virtual std::shared_ptr< ResourcePack< T > > Find( ResourceID id ) = 0;

		/// <summary>
		/// Find and return a resource.
		/// </summary>
		virtual std::shared_ptr< const ResourcePack< T > > Find( ResourceID id ) const = 0;
		*/
										 
		/// <summary>
		/// Returns the number of resources being managed.
		/// </summary>
		virtual size_t Count() const = 0;
	};
}
