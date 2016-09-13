#pragma once

#include <rm/ResourceManagerBase.h>
#include <rm/ISourceFactoryFunctor.h>

namespace rm
{
	/// <summary>
	/// Resouce manager that allows delayed loading and reloading of assets.
	/// This allows us to dynamically load and unload assets during game play.
	/// </summary>
	template< typename T >
	class ResourceManagerAdvanced : public ResourceManagerBase< T >
	{
	public:
		/// <summary>
		/// Constructor for ResourceManager.
		/// Name specifies the name of the resource so we can refer to this when reporting information on the
		/// resources it contains - such as errors.
		/// </summary>
		ResourceManagerAdvanced( std::string name );

		~ResourceManagerAdvanced();

		/// <summary>
		/// Add resource with a specfic ID via a factory.
		/// Specifying a factory allows the resoure to be created as needed, and thus re-creatable upon being lost.
		/// Returns false if a resource already exists with the specified ID. Since the factory is a shared_ptr, and is not retained,
		/// it's lifetime is not incremented.
		/// createImmediately: true to load the resource from source immediately, if we expect to use it immediately.
		/// </summary>
		std::shared_ptr< ResourcePack< T > > AddResource( std::string name, std::shared_ptr< IFactoryFunctor< T > > factory, bool createImmediately = false );

		/// <summary>
		/// Add resource with a specific ID via a source path.
		/// Returns false if a resource already exists with the same ID. It is up to the calling method to treat the return
		/// as a failure or a success based on expectations. For example, if we are loading textures as resource for a 3d model,
		/// it is possible we use the source as the ID, to simplify model loading, and so we can share textures across multiple models,
		/// without loading the texture more than once.
		/// createImmediately: true to load the resource from source immediately, if we expect to use it immediately.
		/// </summary>
 		std::shared_ptr< ResourcePack< T > > AddResource( std::string name, unify::Path source, bool createImmediately = false );

		/// <summary>
		/// Add a factory which takes in a path as a source, so that we can enable the use of the
		/// "add resource from path" feature, which simplifies loading from a path.
		/// </summary>
		void AddSourceFactory( ISourceFactoryFunctor< T > * factory );

	private:
		std::list< std::shared_ptr< ISourceFactoryFunctor< T > > > m_sourceFactories;
	};

	#include <rm/ResourceManagerAdvanced.inl>
}
