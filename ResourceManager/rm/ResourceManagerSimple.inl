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
const std::string ResourceManagerSimple< T >::GetName() const
{
	return m_resourceName;
}				  

template< class T >
void ResourceManagerSimple< T >::Clear()
{
	m_resourceList.clear();
}

template< class T >
std::shared_ptr< T > ResourceManagerSimple< T >::Find( const std::string & id )
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
bool ResourceManagerSimple< T >::Exists( const ResourceID & id ) const
{
	std::map< ResourceID, ResourcePtr >::const_iterator itr = m_resourceList.find( id );
	return !( itr == m_resourceList.end() );
}

// Add an already created resource so the memory is taken over by the manager.
template< class T >
std::shared_ptr< T > ResourceManagerSimple< T >::Add( const std::string & id, T * resource )
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
std::shared_ptr< T > ResourceManagerSimple< T >::Add( const std::string & id, unify::Path source )
{
	// Attempt to find the existing resource.
	ResourcePtr existingResource = Find( id );

	// If exists, return the existing resource. Else create it.
	if( existingResource )
	{
		return existingResource;
	}

	for( auto factory : m_sourceFactories )
	{
		T * product = factory->Produce( source );
		if( product != nullptr )
		{
			auto resource = std::shared_ptr< T >( product );
			m_resourceList[id] = resource;
			return resource;
		}
	}

	throw exception::FailedToCreate( GetName() + " manager: No source path factory found that could produce \"" + id + "\"!" );
}

template< class T >
std::shared_ptr< T > ResourceManagerSimple< T >::Add( qjson::Object json )
{
	ResourceID id = json["name"].ToString();

	// Attempt to find the existing resource.
	ResourcePtr existingResource = Find( id );

	// If exists, return the existing resource. Else create it.
	if( existingResource )
	{
		return existingResource;
	}

	for( auto factory : m_jsonFactories )
	{
		T * product = factory->Produce( json );
		if ( product != nullptr )
		{
			auto resource = std::shared_ptr< T >( product );
			m_resourceList[ id ] = resource;
			return resource;
		}
	}

	throw exception::FailedToCreate( GetName() + " manager: No JSON factory found that could produce \"" + id + "\"!" );
}

template< class T >
std::shared_ptr< T > ResourceManagerSimple< T >::Add( const qxml::Element * node )
{
	std::string id = node->GetStringAttribute( "name" );

	// Attempt to find the existing resource.
	ResourcePtr existingResource = Find( id );

	// If exists, return the existing resource. Else create it.
	if( existingResource )
	{
		return existingResource;
	}

	for( auto factory : m_xmlFactories )
	{
		T * product = factory->Produce( node );
		if( product != nullptr )
		{
			auto resource = std::shared_ptr< T >( product );
			m_resourceList[id] = resource;
			return resource;
		}
	}

	throw exception::FailedToCreate( GetName() + " manager: No XML factory found that could produce \"" + id + "\"!" );
}

template< class T >
size_t ResourceManagerSimple< T >::Count() const
{
	return m_resourceList.size();
}

template< class T >
void ResourceManagerSimple< T >::ForEach( ForEachFunctor & functor )
{
	for( std::map< ResourceID, ResourcePtr >::iterator itr = m_resourceList.begin(); itr != m_resourceList.end(); ++itr )
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
void ResourceManagerSimple< T >::AddFactory( ISourceFactory< T > * factory )
{
	m_sourceFactories.push_back( std::shared_ptr< ISourceFactory< T > >( factory ) );
}

template< typename T >
void ResourceManagerSimple< T >::AddFactory( IXMLFactory< T > * factory )
{
	m_xmlFactories.push_back( std::shared_ptr< IXMLFactory< T > > ( factory ) );
}
