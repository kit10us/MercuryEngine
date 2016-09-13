#pragma once

#include <rm/ResourcePack.h>
#include <map>
#include <list>
#include <string>
#include <memory>
#include <rm/IResourceManager.h>

namespace rm
{
	template< typename T >
	class ResourceManagerBase : public IResourceManager< T >
	{
		struct ci_less : std::binary_function<std::string, std::string, bool>
		{
			// case-independent (ci) compare_less binary function
			struct nocase_compare : public std::binary_function<unsigned char, unsigned char, bool>
			{
				bool operator() ( const unsigned char& c1, const unsigned char& c2 ) const {
					return tolower ( c1 ) < tolower ( c2 );
				}
			};
			bool operator() ( const std::string & s1, const std::string & s2 ) const {
				return std::lexicographical_compare
					( s1.begin (), s1.end (),   // source range
						s2.begin (), s2.end (),   // dest range
						nocase_compare () );  // comparison
			}
		};

	public:
		/// <summary>
		/// Constructor for ResourceManager.
		/// Name specifies the name of the resource so we can refer to this when reporting information on the
		/// resources it contains - such as errors.
		/// </summary>
		ResourceManagerBase( std::string name );

		~ResourceManagerBase() override;

		/// <summary>
		/// Returns the name of the resource being managed.
		/// </summary>
		std::string GetName() const override;

		/// <summary>
		/// Add resource with a specific ID.
		/// Throws an exception if a resource already exists with the same ID, since we are passing in an already allocated resource,
		/// this could lead to a unmanaged resource pointer.
		/// </summary>
		void AddResource( std::string name, T * resource ) override;

		/// <summary>
		/// Returns true if a resource exists with the specified ID.
		/// </summary>
		bool Exists( std::string name ) const override;

		/// <summary>
		/// Find and return a resource.
		/// </summary>
		std::shared_ptr< ResourcePack< T > > Find( std::string name ) override;

		/// <summary>
		/// Find and return a resource.
		/// </summary>
		std::shared_ptr< const ResourcePack< T > > Find( std::string name ) const override;

		/// <summary>
		/// Return the number of resources.
		/// </sumamry>
		size_t Count() const;

	private:
		std::string m_resourceName;
		std::map< std::string, std::shared_ptr< ResourcePack< T > >, ci_less > m_resources;
	};

	#include <rm/ResourceManagerBase.inl>
}
