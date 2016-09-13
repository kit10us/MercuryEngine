
template< typename T >
ResourceManagerAdvanced< T >::ResourceManagerAdvanced( std::string resourceName )
	: ResourceManagerBase( resourceName )
{
}

template< typename T >
ResourceManagerAdvanced< T >::~ResourceManagerAdvanced()
{
}

template< typename T >
std::shared_ptr< ResourcePack< T > > ResourceManagerAdvanced< T >::AddResource( std::string name, std::shared_ptr< IFactoryFunctor< T > > factory, bool createImmediately )
{
	ResourcePack< T > pack;
	bool exists = false;

	// Attempt to find the resource ifit already exists...
	auto itr = m_resources.find( name );
	if ( itr == m_resources.end() )
	{
		// Resource was not found, add a new one.
		pack = ResourcePack< T >( factory );
		m_resources[ name ] = pack;
	}
	else
	{
		// Resource was found, grab it.
		pack = itr->second;
		exists = true;
	}

	// Create the resource if we need it created immediately.
	if ( createImmediately )
	{
		pack.Create();
	}

	return pack;
}

template< typename T >
std::shared_ptr< ResourcePack< T > > ResourceManagerAdvanced< T >::AddResource( std::string name, unify::Path source, bool createImmediately )
{
	return AddResource( id, new SourceFactoryFunctor< T >( this, source ), createImmediately );
}

template< typename T >
void ResourceManagerAdvanced< T >::AddSourceFactory( ISourceFactoryFunctor< T > * factory )
{
	m_sourceFactories.push_back( factory );
}
