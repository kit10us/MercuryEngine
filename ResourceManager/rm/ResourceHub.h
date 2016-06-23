#pragma once
#pragma comment(lib, "ResourceManager")
#include <rm/IResourceManager.h>
#include <unify/Cast.h>
#include <unify/String.h>
#include <map>
#include <memory>

namespace rm
{
	/// <summary>
	/// The Resource Hub is designed as a one-stop for resource management.
	/// It's job is a single point of contact to own all resource managers, 
	/// enabling "smart" loading, or cascaded loading, where an XML load for
	/// one file type and trigger the loading of another type. For example,
	/// a graphics Effect load can trigger the loading of dependant shaders,
	/// textures, etc. This also would support packaging of resources into
	/// groups via XML.
	/// </summary>
	class ResourceHub
	{
	public:
		ResourceHub();
		~ResourceHub();

		/// <summary>
		/// Add a resource manager.
		/// </summary>
		void AddManager( IResourceManagerEarly * manager );

		/// <summary>
		/// Remove all resource managers.
		/// </summary>
		void Clear();

		template< typename T >
		rm::IResourceManager< T > * GetManager( std::string name );

	private:
		/*
		struct CompareNoCase
		{
			bool operator()( const std::string & a, const std::string & b ) const;
		};*/
		std::map< std::string /*Resource name*/, std::shared_ptr< IResourceManagerEarly >, unify::CaseInsensitiveLessThanTest > m_resourceManagers;
	};

	template< typename T >
	IResourceManager< T > * ResourceHub::GetManager( std::string name )
	{
		auto managerPairItr = m_resourceManagers.find( name );
		if ( managerPairItr == m_resourceManagers.end() )
		{
			return nullptr;
		}

		auto managerItr = managerPairItr->second;
		IResourceManagerEarly * rmEarly = managerItr.get();
		auto resolved = unify::polymorphic_downcast< IResourceManager< T > * >( rmEarly );
		return resolved;
	}
}