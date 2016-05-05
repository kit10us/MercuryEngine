// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

template< class T >
ResourceManager< T >::ResourceManager( const dxi::ResourceDesc & resourceDesc, std::shared_ptr< IFactory > factory )
: m_resourceDesc( resourceDesc )
, m_factory( factory )
{
}

template< class T >
ResourceManager< T >::~ResourceManager()
{
	Clear();
}


template< class T >
void ResourceManager< T >::Clear()
{
	m_resourceList.clear();
}

template< class T >
std::shared_ptr< T > ResourceManager< T >::Find( const std::string & id )
{
	for( std::map< ResourceID, ResourcePtr >::iterator itr = m_resourceList.begin(); itr != m_resourceList.end(); ++itr )
	{
		if( _stricmp( (*itr).first.c_str(), id.c_str() ) == 0 )
		{
			return (*itr).second;
		}
	}

	return ResourcePtr();
}

template< class T >
bool ResourceManager< T >::Exists( const ResourceID & id ) const
{
	std::map< ResourceID, ResourcePtr >::const_iterator itr = m_resourceList.find( id );
	return !( itr == m_resourceList.end() );
}

// Add an already created resource so the memory is taken over by the manager.
template< class T >
std::shared_ptr< T > ResourceManager< T >::Add( const std::string & id, T * resource )
{
	// Attempt to find the existing resource.
	ResourcePtr existingResource = Find( id );

	// Fail if we are attempting to add a new resource over an existing one.
	if( existingResource )
	{
		throw unify::Exception( "Attempted to add resource \"" + id + "\", when ID is already in use by a different resource!" );
	}

	// TODO: Insertion?
	ResourcePtr resourcePtr( resource );
	m_resourceList[ id ] = resourcePtr;

	return resourcePtr;
}

template< class T >
std::shared_ptr< T > ResourceManager< T >::Add( const std::string & id, std::string source )
{
	// Attempt to find the existing resource.
	ResourcePtr existingResource = Find( id );

	// If exists, return the existing resource. Else create it.
	if( existingResource )
	{
		return existingResource;
	}

	ResourcePtr resourcePtr( m_factory->Produce( object ) );
	m_resourceList[id] = resourcePtr;

	return resourcePtr;
}

template< class T >
ResourceDesc & ResourceManager< T >::GetResourceDesc()
{
	return m_resourceDesc;
}

template< class T >
size_t ResourceManager< T >::Count() const
{
	return m_resourceList.size();
}

template< class T >
void ResourceManager< T >::ForEach( ForEachFunctor & functor )
{
	for( std::map< ResourceID, ResourcePtr >::iterator itr = m_resourceList.begin(); itr != m_resourceList.end(); ++itr )
	{
		ResourcePtr resource = itr->second;
        functor( resource.get() );
	}
}

