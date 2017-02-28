// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

template< class T >
ResourceManagerSimple< T >::ResourceManagerSimple( std::string resourceName, unify::AssetPaths * assetPaths, ILogger::ptr logger )
	: m_resourceName( resourceName )
	, m_assetPaths( assetPaths )
	, m_logger( logger )
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
	m_resourceMap.clear();
}

template< class T >
std::shared_ptr< T > ResourceManagerSimple< T >::Find( std::string name )
{
	for( auto resource : m_resourceMap )
	{
		if( _stricmp( resource.first.c_str(), name.c_str() ) == 0 )
		{
			return resource.second;
		}
	}

	return ResourcePtr();
}

template< class T >
bool ResourceManagerSimple< T >::Exists( std::string name ) const
{
	std::map< std::string, ResourcePtr >::const_iterator itr = m_resourceMap.find( name );
	return !( itr == m_resourceMap.end() );
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

	ResourcePtr resourcePtr( resource );
	m_resourceMap[ name ] = resourcePtr;
	m_resourceList.push_back( resourcePtr );
	m_resourceNames.push_back( name );

	return resourcePtr;
}

template< class T >
std::shared_ptr< T > ResourceManagerSimple< T >::Add( std::string name, unify::Path source, unify::Path relativePath, void * data )
{
	// Attempt to find the existing resource.
	ResourcePtr existingResource = Find( name );

	// If exists, return the existing resource. Else create it.
	if( existingResource )
	{
		return existingResource;
	}

	std::string extension = source.ExtensionOnly();
	auto factory = m_sourceFactories.find( extension );
	if( factory == m_sourceFactories.end() )
	{
		throw std::string( GetName() + " manager: No factory found that could produce \"" + name + "\"!" );
	}

	if ( m_assetPaths != 0 )
	{
		source = m_assetPaths->FindAsset( source, relativePath );
	}

	Log_WriteLine( GetName() + " manager: adding \"" + name + "\" (" + source.ToString() + ")." );


	auto product = factory->second->Produce( source, data );
	if( product )
	{
		m_resourceMap[ name ] = product;
		m_resourceList.push_back( product );
		m_resourceNames.push_back( name );
		return product;
	}

	throw std::string( GetName() + " manager: No factory found that could produce \"" + name + "\"!" );
}

template< typename T >
void ResourceManagerSimple< T >::AddResource( std::string name, unify::Path path )
{
	Add( name, path );
}

template< class T >
size_t ResourceManagerSimple< T >::Count() const
{
	return m_resourceMap.size();
}

template< class T >
std::string ResourceManagerSimple< T >::GetResourceName( size_t index ) const
{
	return m_resourceNames[ index ];
}
template< class T >
void ResourceManagerSimple< T >::ForEach( ForEachFunctor & functor )
{
	for( std::map< std::string, ResourcePtr >::iterator itr = m_resourceMap.begin(); itr != m_resourceMap.end(); ++itr )
	{
		ResourcePtr resource = itr->second;
        functor( resource.get() );
	}
}

template< typename T >
void ResourceManagerSimple< T >::AddFactory( std::string extension, std::shared_ptr< ISourceFactory< T > > factory )
{
	m_sourceFactories[ ( ( extension[ 0 ] != '.' ) ? "." : "") + extension ] = factory;
}
								 
template< typename T >
void ResourceManagerSimple< T >::Log_Write( std::string text )
{
	if ( m_logger )
	{
		m_logger->Write( text );
	}
}

template< typename T >
void ResourceManagerSimple< T >::Log_WriteLine( std::string text )
{
	if ( m_logger )
	{
		m_logger->WriteLine( text );
	}
}
