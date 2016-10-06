// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

template< class T >
ResourceManagerSimple< T >::ResourceManagerSimple( std::string resourceName )
	: m_resourceName( resourceName )
{
}

template< class T >
ResourceManagerSimple< T >::~ResourceManagerSimple()
{
	Clear();
}

template< class T >
std::string ResourceManagerSimple< T >::GetName() const
{
	return m_resourceName;
}				  

template< class T >
void ResourceManagerSimple< T >::Clear()
{
	m_resourceList.clear();
}

template< class T >
std::shared_ptr< T > ResourceManagerSimple< T >::Find( std::string name )
{
	for( std::map< std::string, ResourcePtr >::iterator itr = m_resourceList.begin(); itr != m_resourceList.end(); ++itr )
	{
		if( _stricmp( (*itr).first.c_str(), name.c_str() ) == 0 )
		{
			return (*itr).second;
		}
	}

	return ResourcePtr();
}

template< class T >
bool ResourceManagerSimple< T >::Exists( std::string name ) const
{
	std::map< std::string, ResourcePtr >::const_iterator itr = m_resourceList.find( name );
	return !( itr == m_resourceList.end() );
}

// Add an already created resource so the memory is taken over by the manager.
template< class T >
std::shared_ptr< T > ResourceManagerSimple< T >::Add( std::string name, T * resource )
{
	// Attempt to find the existing resource.
	ResourcePtr existingResource = Find( name );

	// Fail if we are attempting to add a new resource over an existing one.
	if( existingResource )
	{
		throw unify::Exception( "Attempted to add resource \"" + name + "\", when name is already in use by a different resource!" );
	}

	// TODO: Insertion?
	ResourcePtr resourcePtr( resource );
	m_resourceList[ name ] = resourcePtr;

	return resourcePtr;
}

template< class T >
std::shared_ptr< T > ResourceManagerSimple< T >::Add( std::string name, unify::Path source )
{
	// Attempt to find the existing resource.
	ResourcePtr existingResource = Find( name );

	// If exists, return the existing resource. Else create it.
	if( existingResource )
	{
		return existingResource;
	}

	if ( source.IsExtension( "xml" ) )
	{
		qxml::Document doc( source );
		qxml::Element & element = *doc.GetRoot()->GetFirstChild();
		return Add( name, element );
	}
	else
	{
		std::string extension = source.ExtensionOnly();
		auto factory = m_sourceFactories.find( extension );
		if( factory == m_sourceFactories.end() )
		{
			throw std::string( GetName() + " manager: No source path factory found that could produce \"" + name + "\"!" );
		}

		T * product = factory->second->Produce( source );
		if( product != nullptr )
		{
			auto resource = std::shared_ptr< T >( product );
			m_resourceList[ name ] = resource;
			return resource;
		}
	}

	throw std::string( GetName() + " manager: No source path factory found that could produce \"" + name + "\"!" );
}

template< class T >
std::shared_ptr< T > ResourceManagerSimple< T >::Add( std::string name, const qxml::Element & node )
{
	// Attempt to find the existing resource.
	ResourcePtr existingResource = Find( name );

	// If exists, return the existing resource. Else create it.
	if( existingResource )
	{
		return existingResource;
	}

	auto factory = m_xmlFactoriesMap[node.GetTagName()];
	T * product = factory->Produce( node );
	if( product != nullptr )
	{
		auto resource = std::shared_ptr< T >( product );
		m_resourceList[ name ] = resource;
		return resource;
	}

	throw std::string( GetName() + " manager: No XML factory found that could produce \"" + name + "\"!" );
}

template< class T >
std::shared_ptr< T > ResourceManagerSimple< T >::Add( const qxml::Element & node )
{
	if( node.HasAttributes( "name,source" ) )
	{
		return Add( node.GetAttribute< std::string >( "name" ), unify::Path( node.GetDocument()->GetPath().DirectoryOnly(), node.GetAttribute< std::string >( "source" ) ) );
	}

	std::string name = node.GetAttribute( "name" )->GetString();

	return Add( name, node );
}

template< typename T >
void ResourceManagerSimple< T >::AddResource( const qxml::Element & element )
{
	Add( element );
}

template< typename T >
void ResourceManagerSimple< T >::AddResource( std::string name, const qxml::Element & element )
{
	Add( name, element );
}

template< typename T >
void ResourceManagerSimple< T >::AddResource( std::string name, unify::Path path )
{
	Add( name, path );
}

template< class T >
std::shared_ptr< T > ResourceManagerSimple< T >::Add( qjson::Object json )
{
	std::string name = json["name"].ToString();

	// Attempt to find the existing resource.
	ResourcePtr existingResource = Find( name );

	// If exists, return the existing resource. Else create it.
	if( existingResource )
	{
		return existingResource;
	}

	for( auto factory : m_jsonFactories )
	{
		T * product = factory->Produce( json );
		if( product != nullptr )
		{
			auto resource = std::shared_ptr< T >( product );
			m_resourceList[ name ] = resource;
			return resource;
		}
	}

	throw std::string( GetName() + " manager: No JSON factory found that could produce \"" + name + "\"!" );
}

template< class T >
size_t ResourceManagerSimple< T >::Count() const
{
	return m_resourceList.size();
}

template< class T >
void ResourceManagerSimple< T >::ForEach( ForEachFunctor & functor )
{
	for( std::map< std::string, ResourcePtr >::iterator itr = m_resourceList.begin(); itr != m_resourceList.end(); ++itr )
	{
		ResourcePtr resource = itr->second;
        functor( resource.get() );
	}
}

template< typename T >
void ResourceManagerSimple< T >::AddFactory( IJsonFactory< T > * factory )
{
	m_jsonFactories.push_back( std::shared_ptr< IJsonFactory < T > > ( factory ) );
}

template< typename T >
void ResourceManagerSimple< T >::AddFactory( std::string extension, ISourceFactory< T > * factory )
{
	m_sourceFactories[ ( ( extension[ 0 ] != '.' ) ? "." : "") + extension ] = std::shared_ptr< ISourceFactory< T > >( factory );
}

template< typename T >
void ResourceManagerSimple< T >::AddFactory( std::string tagName, IXMLFactory< T > * factory )
{
	std::shared_ptr< IXMLFactory< T > > sharedFactory( factory );
	m_xmlFactories.push_back( sharedFactory );
	m_xmlFactoriesMap[ tagName ] = sharedFactory;
}
